% =========================================================================
% PROJECT: GAIT ANALYSIS - FINAL (STEP COMPARISON ADDED)
% FILE: Analysis.m
% DESCRIPTION: 
%   1. Visualize File 1 (4 Separate Windows).
%   2. Compare STEP COUNTS across Files 1-10 (New Feature).
%   3. Calculate Step Time Stability & Total Error.
% =========================================================================

clc; clear; close all;

%% PART 1: CONFIGURATION
colors = [0.9 0.9 0.9;   % 0: Flat
          1.0 1.0 0.8;   % 1: Lift
          0.8 1.0 0.8;   % 2: Swing
          1.0 0.8 0.8];  % 3: Strike

file_range = 1:10;     % Analyze files 1 to 10
target_file_plot = 2;  % Visualize file 1

%% PART 2: DATA VISUALIZATION (FILE 1 - SEPARATE FIGURES)
fprintf('>>> Processing visualization for file %d.CSV...\n', target_file_plot);
filename = sprintf('%d.CSV', target_file_plot);

if isfile(filename)
    data = readmatrix(filename, 'Delimiter', ',');
    if size(data, 2) < 6
        error('File %s lacks columns. Check CSV format.', filename);
    end

    t = data(:, 1);
    fsr = data(:, 2);
    Am = data(:, 3);
    GyroMag = data(:, 4);
    Phase = data(:, 5);
    Step = data(:, 6);
    
    % --- FIG 1: FSR ---
    figure('Name', '1. FSR Sensor', 'Color', 'w', 'Position', [50, 500, 600, 350]);
    ax1 = gca; hold on;
    plot(t, fsr, 'b'); drawPhaseBackground(t, Phase, colors); plot(t, fsr, 'b');
    title(['File ', filename, ': FSR Sensor']); grid on;

    % --- FIG 2: ACCEL ---
    figure('Name', '2. Acceleration', 'Color', 'w', 'Position', [50, 50, 600, 350]);
    ax2 = gca; hold on;
    plot(t, Am, 'k'); drawPhaseBackground(t, Phase, colors); plot(t, Am, 'k');
    title(['File ', filename, ': Acceleration']); grid on;

    % --- FIG 3: GYRO ---
    figure('Name', '3. Gyroscope', 'Color', 'w', 'Position', [700, 500, 600, 350]);
    ax3 = gca; hold on;
    plot(t, GyroMag, 'm'); drawPhaseBackground(t, Phase, colors); plot(t, GyroMag, 'm');
    yline(1.5, '--r');
    title(['File ', filename, ': Gyro Swing']); grid on;

    % --- FIG 4: STEPS ---
    figure('Name', '4. Step Count', 'Color', 'w', 'Position', [700, 50, 600, 350]);
    ax4 = gca; hold on;
    stairs(t, Step, 'r', 'LineWidth', 2); drawPhaseBackground(t, Phase, colors); stairs(t, Step, 'r', 'LineWidth', 2);
    title(['File ', filename, ': Step Accumulation']); grid on;
    
    linkaxes([ax1, ax2, ax3, ax4], 'x');
end

%% PART 3: DATA AGGREGATION (FILES 1-10)
fprintf('\n>>> Analyzing Files 1-%d...\n', max(file_range));

results = table([], [], [], [], [], 'VariableNames', ...
    {'FileName', 'TotalSteps', 'Avg_Step_Time', 'Step_Time_Stability', 'Cadence'});

all_step_durations = []; 

for i = file_range
    fname = sprintf('%d.CSV', i);
    if isfile(fname)
        try
            raw = readmatrix(fname, 'Delimiter', ',');
            if size(raw, 2) < 6, continue; end
            
            time_col = raw(:,1);
            step_col = raw(:,6);
            
            % 1. Get Total Steps
            total_steps = max(step_col);
            
            % 2. Calculate Timing
            step_indices = find(diff(step_col) > 0);
            if length(step_indices) > 1
                step_durations = diff(time_col(step_indices));
                all_step_durations = [all_step_durations; step_durations];
                
                avg_time = mean(step_durations);
                std_dev = std(step_durations); 
                cadence = (total_steps / (time_col(end) - time_col(1))) * 60;
            else
                avg_time = 0; std_dev = 0; cadence = 0;
            end
            
            results = [results; {fname, total_steps, avg_time, std_dev, cadence}];
        catch
        end
    end
end

disp(results);

%% PART 4: STEP COUNT COMPARISON & FINAL REPORT
if ~isempty(results)
    % --- A. STEP COUNT COMPARISON CHART ---
    figure('Name', 'Step Count Comparison (Files 1-10)', 'Color', 'w');
    bar(results.TotalSteps, 'FaceColor', [0.2 0.6 0.8]);
    yline(mean(results.TotalSteps), '--r', 'Average Steps', 'LineWidth', 2);
    xlabel('File Number'); ylabel('Total Steps Detected');
    xticklabels(results.FileName);
    title('COMPARISON: Total Steps per File');
    grid on;
    
    % --- B. CALCULATE ERRORS ---
    % 1. Step Count Consistency (Độ lệch về số lượng bước)
    mean_steps = mean(results.TotalSteps);
    std_steps_count = std(results.TotalSteps);
    
    % 2. Step Time Consistency (Độ lệch về thời gian bước)
    global_time_error = std(all_step_durations);

    fprintf('\n=================================================\n');
    fprintf('   FINAL ERROR REPORT (FILES 1-10)\n');
    fprintf('=================================================\n');
    
    fprintf('1. STEP COUNT CONSISTENCY (Độ ổn định bộ đếm):\n');
    fprintf('   - Average Steps per File : %.2f steps\n', mean_steps);
    fprintf('   - Step Count Error (Std) : %.4f\n', std_steps_count);
    if std_steps_count == 0
        fprintf('     (PERFECT: All files have the exact same step count)\n');
    elseif std_steps_count < 1
        fprintf('     (EXCELLENT: Very consistent counting)\n');
    else
        fprintf('     (WARNING: Step count varies between files)\n');
    end
    
    fprintf('-------------------------------------------------\n');
    fprintf('2. WALKING RHYTHM (Độ đều bước chân):\n');
    fprintf('   - Average Step Time      : %.4f s\n', mean(all_step_durations));
    fprintf('   - Global Rhythm Error    : %.4f s\n', global_time_error);
    fprintf('=================================================\n');
end

%% HELPER
function drawPhaseBackground(t, phase, colors)
    yl = ylim; 
    diff_phase = diff(phase);
    change_idx = find(diff_phase ~= 0);
    start_idx = [1; change_idx + 1];
    end_idx = [change_idx; length(phase)];
    for i = 1:length(start_idx)
        current_p = phase(start_idx(i));
        if current_p >= 0 && current_p <= 3
            patch([t(start_idx(i)) t(end_idx(i)) t(end_idx(i)) t(start_idx(i))], ...
                  [yl(1) yl(1) yl(2) yl(2)], colors(current_p + 1, :), ...
                  'EdgeColor', 'none', 'FaceAlpha', 0.5); 
        end
    end
    ylim(yl); 
end