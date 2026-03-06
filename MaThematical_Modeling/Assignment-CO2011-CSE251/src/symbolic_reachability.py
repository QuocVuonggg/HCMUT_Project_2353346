import time
import tracemalloc
from collections import deque, defaultdict
from dd.autoref import BDD

def get_connected_components(net):
    places = set(net.places.keys())
    transitions = set(net.transitions.keys())
    
    adj = defaultdict(set)
    for t_id in net.transitions:
        for p in net.pre[t_id]:
            adj[t_id].add(p)
            adj[p].add(t_id)
        for p in net.post[t_id]:
            adj[t_id].add(p)
            adj[p].add(t_id)
            
    components = []
    visited = set()
    all_nodes = list(places) + list(transitions)
    
    for node in all_nodes:
        if node in visited:
            continue
            
        component_places = set()
        component_transitions = set()
        
        queue = deque([node])
        visited.add(node)
        
        if node in places: component_places.add(node)
        else: component_transitions.add(node)
        
        while queue:
            curr = queue.popleft()
            for neighbor in adj[curr]:
                if neighbor not in visited:
                    visited.add(neighbor)
                    queue.append(neighbor)
                    if neighbor in places:
                        component_places.add(neighbor)
                    else:
                        component_transitions.add(neighbor)
                        
        if component_places or component_transitions:
            components.append((list(component_places), list(component_transitions)))
            
    return components

def solve_component(bdd, net, comp_places, comp_transitions):
    p_vars = {}
    sorted_comp_places = sorted(comp_places)
    for p in sorted_comp_places:
        var_name = f'x_{p}'
        bdd.declare(var_name)
        p_vars[p] = bdd.var(var_name)

    M = bdd.true
    initial_set = set(net.initial_marking)
    for p in sorted_comp_places:
        if p in initial_set:
            M &= p_vars[p]
        else:
            M &= ~p_vars[p]

    logic = []
    for t_id in comp_transitions:
        pre = net.pre[t_id]
        post = net.post[t_id]
        
        pre_cond = bdd.true
        for p in pre: pre_cond &= p_vars[p]
            
        consumed = [p for p in pre if p not in post]
        produced = [p for p in post if p not in pre]
        vars_to_forget = {f'x_{p}' for p in consumed + produced}
        
        if not vars_to_forget: continue
            
        post_cond = bdd.true
        for p in consumed: post_cond &= ~p_vars[p]
        for p in produced: post_cond &= p_vars[p]
            
        logic.append((pre_cond, vars_to_forget, post_cond))

    while True:
        M_start = M
        for pre, vars_forget, post in logic:
            ready = M & pre
            if ready == bdd.false: continue
            
            succ = bdd.exist(vars_forget, ready) & post
            M = M | succ
            
        if M == M_start:
            break
            
    return M

def run_symbolic_reachability(net):
    start_time = time.time()
    tracemalloc.start()

    components = get_connected_components(net)
        
    bdd = BDD()
    bdd.configure(reordering=False) 

    for p in net.places:
        bdd.declare(f'x_{p}')

    global_bdd = bdd.true 
    total_count = 1
    
    for comp_places, comp_transitions in components:
        M_local = solve_component(bdd, net, comp_places, comp_transitions)
        global_bdd = global_bdd & M_local
        
        sub_count = M_local.count(nvars=len(comp_places))
        total_count *= sub_count
        
    end_time = time.time()
    current, peak_memory = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    duration = end_time - start_time
    
    return global_bdd, total_count, duration, peak_memory