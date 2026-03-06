import time
import pulp

def run_reachability_optimization(net, reachable_bdd=None):
    start_time = time.time()
    
    places = list(net.places.keys())
    transitions = list(net.transitions.keys())
    
    C = {}
    connected_t = {p: [] for p in places} 

    def add_connection(p, t, weight_change):
        if (p, t) not in C:
            C[(p, t)] = 0
            connected_t[p].append(t)
        C[(p, t)] += weight_change

    for t, p_set in net.pre.items():
        for p in p_set:
            add_connection(p, t, -1)

    for t, p_set in net.post.items():
        for p in p_set:
            add_connection(p, t, 1)

    M0 = {p: 0 for p in places}
    for p in net.initial_marking:
        if p in M0: M0[p] = 1

    prob = pulp.LpProblem("Fast_Reachability", pulp.LpMaximize)
    
    vars_M = pulp.LpVariable.dicts("M", places, lowBound=0, cat='Continuous')
    vars_sigma = pulp.LpVariable.dicts("sigma", transitions, lowBound=0, cat='Continuous')
    
    prob += pulp.lpSum([vars_M[p] for p in places])
    
    for p in places:
        if not connected_t[p]: # Isolated place
            prob += (vars_M[p] == M0[p])
        else:
            dot_product = pulp.lpSum([C[(p, t)] * vars_sigma[t] for t in connected_t[p]])
            prob += (vars_M[p] == M0[p] + dot_product)


    prob.solve(pulp.PULP_CBC_CMD(msg=False))
    
    duration = time.time() - start_time
    
    if pulp.LpStatus[prob.status] == 'Optimal':
        max_val = int(pulp.value(prob.objective))
        
        best_marking_set = set()
        for p in places:
            if pulp.value(vars_M[p]) > 0.5:
                best_marking_set.add(p)
                
        return max_val, frozenset(best_marking_set), duration
    else:
        return 0, None, duration