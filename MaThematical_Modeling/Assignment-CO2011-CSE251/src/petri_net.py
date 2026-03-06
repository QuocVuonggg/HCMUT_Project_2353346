class PetriNet:
    def __init__(self):
        self.places = {}
        self.transitions = {}
        self.initial_marking = frozenset()
        
        self.pre = {}
        self.post = {}
        
        self.arcs_info = {}

    def add_place(self, p_id, label=None):
        self.places[p_id] = label or p_id

    def add_transition(self, t_id, label=None):
        self.transitions[t_id] = label or t_id
        self.pre[t_id] = set()
        self.post[t_id] = set()

    def add_arc(self, arc_id, source, target):
        if source in self.places and target in self.transitions:
            self.pre[target].add(source)
        elif source in self.transitions and target in self.places:
            self.post[source].add(target)
        else:
            raise ValueError(f"Invalid arc {arc_id}: {source} -> {target} (Must be P->T or T->P)")
            
        self.arcs_info[arc_id] = (source, target)

    def set_initial_marking(self, p_id):
        current = set(self.initial_marking)
        current.add(p_id)
        self.initial_marking = frozenset(current)

    # --- Helper functions for Task 2 & 3 ---
    
    def get_sorted_places(self):
        return sorted(list(self.places.keys()))

    def get_enabled_transitions(self, current_marking):
        enabled = []
        for t_id in self.transitions:
            if self.pre[t_id].issubset(current_marking):
                enabled.append(t_id)
        return enabled

    def fire_transition(self, current_marking, t_id):
        new_marking = set(current_marking)
        new_marking -= self.pre[t_id]
        new_marking |= self.post[t_id]
        return frozenset(new_marking)

    def get_statistics(self):
        lines = []
        
        p_list = []
        for p_id, p_name in self.places.items():
            tokens = 1 if p_id in self.initial_marking else 0
            p_list.append(f"{p_name}({tokens})")
        
        lines.append(f"Places ({len(self.places)}): {', '.join(p_list)}")

        t_list = list(self.transitions.values())
        lines.append(f"Transitions ({len(self.transitions)}): {', '.join(t_list)}")

        a_list = []
        for a_id, (source, target) in self.arcs_info.items():
            a_list.append(f"{source}->{target}")
            
        lines.append(f"Flow Relations/Arcs ({len(self.arcs_info)}): {', '.join(a_list)}")

        return "\n\n".join(lines)