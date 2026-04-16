import os
from sage.graphs.graph_generators import graphs
from tqdm import tqdm
# Use Sage's built-in generator
counter=0
iterator = graphs.planar_graphs(12,minimum_degree=5)
for i,G in tqdm(enumerate(iterator)):
        if G.is_connected() and G.is_regular(5):
                counter+=1
                G.plot(layout='planar').save(os.path.join("output_gen",f"my_graph_{i}.png"))
print(counter)