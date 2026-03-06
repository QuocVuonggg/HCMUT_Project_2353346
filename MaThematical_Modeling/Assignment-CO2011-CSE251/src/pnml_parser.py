import xml.etree.ElementTree as ET
from petri_net import PetriNet

def parse_pnml(file_path):
    try:
        tree = ET.parse(file_path)
        root = tree.getroot()
    except FileNotFoundError:
        print(f"Error: File '{file_path}' not found.")
        return None
    except ET.ParseError:
        print(f"Error: Failed to parse XML in '{file_path}'.")
        return None

    net = PetriNet()

    # Handle XML Namespaces (strip them for easier parsing)
    for neighbor in root.iter():
        if '}' in neighbor.tag:
            neighbor.tag = neighbor.tag.split('}', 1)[1]

    # Locate the <net> tag
    net_element = root.find('net')
    if net_element is None:
        net_element = root

    # 1. Parse Places
    for place in net_element.findall('.//place'):
        p_id = place.get('id')
        
        # Check for Initial Marking text value
        init_mark = place.find('initialMarking')
        is_marked = False
        if init_mark is not None:
            text = init_mark.find('text')
            if text is not None and text.text and int(text.text) > 0:
                is_marked = True
        
        net.add_place(p_id)
        if is_marked:
            net.set_initial_marking(p_id)

    # 2. Parse Transitions
    for trans in net_element.findall('.//transition'):
        t_id = trans.get('id')
        net.add_transition(t_id)

    # 3. Parse Arcs
    for arc in net_element.findall('.//arc'):
        a_id = arc.get('id')
        source = arc.get('source')
        target = arc.get('target')
        
        try:
            net.add_arc(a_id, source, target)
        except ValueError as e:
            print(e)
            return None 

    return net