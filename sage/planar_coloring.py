import os
import copy
import math
from sage.graphs.graph import Graph
from sage.graphs.graph_generators import graphs
from sage.plot.colors import rainbow
from sage.plot.text import text

def load_graph(file_name):
    G = Graph()
    pos_dict = {}

    # Open the file and read the lines
    with open(os.path.join("input_graphs",file_name), "r") as f:
        # Read lines and ignore any empty trailing lines
        lines = [line.strip() for line in f if line.strip()]

    n = int(lines[0])

    for i in range(n):
        parts = lines[i + 1].split()

        # Extract coordinates
        pos_dict[i] = (float(parts[0]), float(parts[1]))

        # Extract edges
        for neighbor in parts[2:]:
            G.add_edge(i, int(neighbor))

    # Apply layout and show
    G.set_pos(pos_dict)
    return G

def get_smallest_angle(G, pos):
    """Calculates the tightest angle in a given layout."""
    min_angle = float('inf')
    for v in G.vertices():
        neighbors = G.neighbors(v)
        if len(neighbors) < 2:
            continue

        # Get angles to all neighbors
        angles = sorted(math.atan2(pos[n][1] - pos[v][1], pos[n][0] - pos[v][0]) % (2 * math.pi)
                        for n in neighbors)

        # Check differences between adjacent angles circularly
        for i in range(len(angles)):
            diff = (angles[(i + 1) % len(angles)] - angles[i]) % (2 * math.pi)
            if diff < min_angle:
                min_angle = diff

    return min_angle


def best_planar_layout(G):
    best_pos = None
    best_score = -1

    # Iterate all edges (ignoring labels if any)
    for u, v in G.edges(labels=False, sort=False):
        try:
            pos = G.layout_planar(external_face=(u, v))
            score = get_smallest_angle(G, pos)

            if score > best_score:
                best_score = score
                best_pos = pos
        except ValueError:
            # Skip if the algorithm fails on a specific edge for any reason
            continue

    return best_pos


def find_wagner_minor(graph):
    if graph.is_planar():
        return "Graph is planar."

    K5 = graphs.CompleteGraph(5)
    K33 = graphs.CompleteBipartiteGraph(3, 3)

    # Check for K5 minor
    mapping_k5 = graph.minor(K5)
    if mapping_k5:
        print("Found K5 minor. Contraction sets:", mapping_k5)
        return mapping_k5

    # Check for K3,3 minor
    mapping_k33 = graph.minor(K33)
    if mapping_k33:
        print("Found K3,3 minor. Contraction sets:", mapping_k33)
        return mapping_k33

    return None

file_name="graph_19195"
graph=load_graph(f"{file_name}.txt")
if not os.path.exists("out_planarity_coloring"):
    os.makedirs("out_planarity_coloring")
graph.plot().save(os.path.join("out_planarity_coloring", f"{file_name}.png"))
classes = graph.coloring()
# 3. Assign a distinct color to each set
color_list = rainbow(len(classes))
v_colors = {color_list[i]: vertices for i, vertices in enumerate(classes)}
# 4. Plot the graph with the applied colors
graph.plot(vertex_colors=v_colors).save(os.path.join("out_planarity_coloring", f"{file_name}_coloring.png"))
is_planar, K = graph.is_planar(kuratowski=True)
if is_planar:
    best_pos = best_planar_layout(graph)
    graph.set_pos(best_pos)
    graph.plot().save(os.path.join("out_planarity_coloring", f"{file_name}_planar.png"))
    exit()

K.plot().save(os.path.join("out_planarity_coloring", f"{file_name}_kuratowski.png"))
# 1. Identify core vertices
core_verts = [v for v in K.vertices() if K.degree(v) > 2]
paths = []
visited_edges = set()

# 2. Extract paths
for v in core_verts:
    for n in K.neighbors(v):
        edge_key = frozenset([v, n])
        if edge_key in visited_edges:
            continue

        path = [v, n]
        visited_edges.add(edge_key)
        curr = n

        while K.degree(curr) == 2:
            neighbors = K.neighbors(curr)
            next_v = neighbors[0] if neighbors[0] != path[-2] else neighbors[1]
            visited_edges.add(frozenset([curr, next_v]))
            path.append(next_v)
            curr = next_v

        if v < curr:
            paths.append(path)

# 3. Create the abstract core graph
core_G = Graph()
core_G.add_vertices(core_verts)
edge_labels = {}

for path in paths:
    start_v, end_v = path[0], path[-1]
    core_G.add_edge(start_v, end_v)
    # Store degree-2 vertices as a comma-separated string
    if len(path) > 2:
        edge_labels[(start_v, end_v)] = ", ".join(str(x) for x in path[1:-1])

# 4. Position the core vertices
plot_name = f"{file_name}_kuratowski.png"
pos_dict = {}
if len(core_verts) == 5:
    for i, v in enumerate(core_verts):
        pos_dict[v] = (math.cos(2 * math.pi * i / 5), math.sin(2 * math.pi * i / 5))
    plot_name = f"{file_name}_kuratowski_K5.png"
elif len(core_verts) == 6:
    left, right = core_G.bipartite_sets()
    for i, v in enumerate(left): pos_dict[v] = (0, i)
    for i, v in enumerate(right): pos_dict[v] = (1, i)
    plot_name = f"{file_name}_kuratowski_K33.png"

core_G.set_pos(pos_dict)

# 5. Generate base plot
P = core_G.plot(vertex_size=400, vertex_labels=True)

# 6. Calculate angles and overlay aligned text near the start vertex
for (u, v), lbl in edge_labels.items():
    x1, y1 = pos_dict[u]
    x2, y2 = pos_dict[v]

    # Position 20% along the edge from the start vertex to avoid the center
    lx = x1 + 0.2 * (x2 - x1)
    ly = y1 + 0.2 * (y2 - y1)

    # Angle calculation
    angle_rad = math.atan2(y2 - y1, x2 - x1)
    angle_deg = math.degrees(angle_rad)

    if angle_deg > 90:
        angle_deg -= 180
    elif angle_deg < -90:
        angle_deg += 180

    P += text(lbl, (lx, ly), rotation=angle_deg,
              color='blue', fontsize=12, zorder=5)

# Disable axes when saving
P.save(os.path.join("out_planarity_coloring", plot_name), axes=False)

mapping=find_wagner_minor(graph)
print(mapping)
minor_G = copy.copy(graph)
# 1. Identify all vertices used in the minor mapping
used_vertices = set()
for nodes in mapping.values():
    used_vertices.update(nodes)

# 2. Delete any vertices from the graph that are NOT part of the minor
for v in list(minor_G.vertices()):
    if v not in used_vertices:
        minor_G.delete_vertex(v)

# 3. Merge the vertex sets
for nodes in mapping.values():
    if len(nodes) > 1:
        minor_G.merge_vertices(nodes)

# 4. Relabel ALL remaining vertices to strings to fix the sorting TypeError
relabel_dict = {}
for nodes in mapping.values():
    # merge_vertices always retains the first node in the list
    relabel_dict[nodes[0]] = ",".join(map(str, sorted(nodes)))

minor_G.relabel(relabel_dict)

# 5. Plot based on K5 or K3,3
if minor_G.order() == 5:
    plot_path = os.path.join("out_planarity_coloring", f"{file_name}_wagner_K5.png")
    minor_G.plot(layout='circular', vertex_size=800).save(plot_path)
    print("Saved K5 Wagner minor.")

elif minor_G.order() == 6:
    left, right = minor_G.bipartite_sets()
    pos_dict = {}

    for i, v in enumerate(left): pos_dict[v] = (0, i)
    for i, v in enumerate(right): pos_dict[v] = (1, i)

    minor_G.set_pos(pos_dict)
    plot_path = os.path.join("out_planarity_coloring", f"{file_name}_wagner_K33.png")
    minor_G.plot(vertex_size=800).save(plot_path)
    print("Saved K3,3 Wagner minor.")
