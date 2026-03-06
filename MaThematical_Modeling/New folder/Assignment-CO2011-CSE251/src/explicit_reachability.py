import time
import tracemalloc
from collections import deque

def run_explicit_reachability(net):
    """
    Task 2: Compute reachable markings using explicit BFS.
    Input: PetriNet object
    Output: (Number of states, Execution time)
    """
    start_time = time.time()
    tracemalloc.start()
    
    # 1. Initial State
    initial_m = net.initial_marking
    
    # 2. BFS Initialization
    visited = set()             # Stores visited markings to prevent loops
    queue = deque([initial_m])  # BFS Queue
    
    visited.add(initial_m)
    
    # 3. Main BFS Loop
    while queue:
        current_m = queue.popleft()
        
        # Get enabled transitions for current marking
        enabled_transitions = net.get_enabled_transitions(current_m)
        
        for t_id in enabled_transitions:
            # Compute next marking
            next_m = net.fire_transition(current_m, t_id)
            
            # If new state, add to visited and queue
            if next_m not in visited:
                visited.add(next_m)
                queue.append(next_m)
                
    end_time = time.time()
    current, peak_memory = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    duration = end_time - start_time
    
    return len(visited), duration, peak_memory