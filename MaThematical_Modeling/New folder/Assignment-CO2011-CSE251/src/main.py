import os
import argparse
import time

try:
    from pnml_parser import parse_pnml
    from explicit_reachability import run_explicit_reachability
    from symbolic_reachability import run_symbolic_reachability
    from deadlock_detection import run_deadlock_detection, find_deadlock_with_ilp_bdd
    from reachability_optimization import run_reachability_optimization
except ImportError:
    from src.pnml_parser import parse_pnml
    from src.explicit_reachability import run_explicit_reachability
    from src.symbolic_reachability import run_symbolic_reachability
    from src.deadlock_detection import run_deadlock_detection, find_deadlock_with_ilp_bdd
    from src.reachability_optimization import run_reachability_optimization

# --- HELPER FUNCTIONS ---

def format_duration(seconds):
    if seconds < 1:
        return f"{seconds * 1000:.4f} ms"
    elif seconds < 60:
        return f"{seconds:.4f} s"
    else:
        minutes = seconds / 60
        return f"{minutes:.2f} min"

def format_memory(bytes_val):
    return f"{bytes_val / 1048576:.4f} MB"

# --- TASK WRAPPERS ---

def run_task_1(net):
    print(f"--- Task 1: PNML parsing ---")
    print()
        
    print(net.get_statistics())
    print()
    
    n_p = len(net.places)
    n_t = len(net.transitions)
    n_a = len(net.arcs_info) 
    n_m0 = len(net.initial_marking)
    
    res_str = f"Places: {n_p}, Transitions: {n_t}, Arcs: {n_a}, Initial Markings: {n_m0}"
    print(f"{res_str}")
    return res_str

def run_task_2(net):
    print(f"--- Task 2: Explicit reachability ---")
    count, duration, memory = run_explicit_reachability(net)
    
    time_str = format_duration(duration)
    mem_str = format_memory(memory)
    
    print(f"Total Reachable Markings: {count}")
    print(f"Running Time: {time_str}")
    print(f"Peak Memory Usage: {mem_str}")
    
    return f"Reachable Markings: {count}, Time: {time_str}, Memory: {mem_str}"

def run_task_3(net):
    print(f"--- Task 3: BDD-based reachability ---")
    reachable_bdd, count, duration, memory = run_symbolic_reachability(net)
    
    time_str = format_duration(duration)
    mem_str = format_memory(memory)
    
    print(f"Total Reachable Markings: {count}")
    print(f"Running Time: {time_str}")
    print(f"Peak Memory Usage: {mem_str}")
    
    return reachable_bdd, f"Reachable Markings: {count}, Time: {time_str}, Memory: {mem_str}"

def run_task_4(net, reachable_bdd=None):
    print(f"--- Task 4: ILP + BDD deadlock detection ---")
    
    trace = None
    duration = 0
    found = False

    if reachable_bdd is not None:
        start_time = time.time()
        trace = find_deadlock_with_ilp_bdd(net, reachable_bdd, verbose=False)
        duration = time.time() - start_time
        found = (trace is not None)
    else:
        found, trace, duration = run_deadlock_detection(net, verbose=False)

    time_str = format_duration(duration)
    
    status_str = "Safe"
    if found:
        print(f"Deadlock found: {trace}")
        status_str = "Deadlock Found"
    else:
        print(f"No deadlock found (Safe).")
        
    print(f"Running Time: {time_str}")
        
    return f"{status_str}, Time: {time_str}"

def run_task_5(net, reachable_bdd):
    print(f"--- Task 5: Reachable optimization ---")
    max_val, marking, duration = run_reachability_optimization(net, reachable_bdd)
    time_str = format_duration(duration)
    print(f"Running Time: {time_str}")
    
    if marking is not None:
        print(f"Max Objective Value: {max_val}")
        marking_list = sorted(list(marking))
        marking_str = f"{{{', '.join(marking_list)}}}"
        print(f"Optimal Marking: {marking_str}")
        res_val = str(max_val)
    else:
        print("Max Objective Value: N/A")
        res_val = "N/A"
    
    return f"Max: {res_val}, Time: {time_str}"

# --- MAIN ---

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--task', type=str, default='1')
    parser.add_argument('--file', type=str, default='tc1.pnml')
    args = parser.parse_args()

    current_src_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(current_src_dir)
    file_path = os.path.join(project_root, 'tc', args.file)

    if not os.path.exists(file_path):
        print(f"Error: File not found: {file_path}")
        return

    print(f"Testcase: {args.file}\n")
    net = parse_pnml(file_path)
    if not net: return

    task_arg = args.task.lower()

    if task_arg == 'all':
        res1 = run_task_1(net)
        print("\n" + "-" * 50 + "\n")
        
        res2 = run_task_2(net)
        print("\n" + "-" * 50 + "\n")
        
        bdd, res3 = run_task_3(net)
        print("\n" + "-" * 50 + "\n")
        
        res4 = run_task_4(net, bdd)
        print("\n" + "-" * 50 + "\n")
        
        res5 = run_task_5(net, bdd)
        
        print("\n" + "=" * 100)
        print(f"{'SUMMARY':^100}")
        print("=" * 100)
        
        print(f"Testcase: {args.file}")
        
        print(f"Task 1 - PNML parsing: {res1}")
        print(f"Task 2 - Explicit reachability: {res2}")
        print(f"Task 3 - BDD-based reachability: {res3}")
        print(f"Task 4 - ILP + BDD deadlock detection: {res4}")
        print(f"Task 5 - Reachable optimization: {res5}")
        print("-" * 50)
        
    else:
        try:
            t = int(task_arg)
        except ValueError:
            return

        if t == 1: run_task_1(net)
        elif t == 2: run_task_2(net)
        elif t == 3: run_task_3(net)
        elif t == 4: run_task_4(net, None) 
        elif t == 5:
            bdd, _ = run_symbolic_reachability(net)[0:2]
            run_task_5(net, bdd)

if __name__ == "__main__":
    main()