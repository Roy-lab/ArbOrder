# ArbOrder
This is a c++ function that is used to reorder all arboretum or escarole output files based on the cluster means. This is an improvement to the orthogroup code which makes assumption on the conserved cluster order across multiple difference mean matrices. It additionally identifies features with values in range min_k-1 to k-1 (e.g. zero indexed values) and writes these veatures per cluster into the directory feature_assign <min_k> <k> 


## Usage

```bash
./feature_assigner <arb_output_dir> <tree_file> <min_k> <k> <outdir>
```

| Argument         | Description                                                                                                               |
| ---------------- | ------------------------------------------------------------------------------------------------------------------------- |
| `arb_output_dir` | Directory containing `allcelltypes_clusterassign_lca_brk.txt`, `clustermeans.txt`, and transition matrices (`<Cluster>`). |
| `tree_file`      | The input tree provided to **Arboretum** or **Escarole**.                                                                 |
| `min_k`          | The minimum category (k value) to assign to a cluster.                                                                    |
| `k`              | The number of clusters.                                                                                                   |
| `outdir`         | Output directory where results will be saved.                                                                             |


#### A Note on Indexing 
allcelltypes_clusterassign_lca_brk.txt utilizes zero indexing. That is for 5 clusters (k = 5), the sorted outputs will have assignments: 

0 (low)
1 (low - medium) 
2 (medium)
3 (high - medium)
4 (high) 

representing the various cluster means.  I did not want to have to actively think about zero indexing when assigning clusters and just wanted to use the k option.  

That is if  min_k = 3, and k = 5. Features will be assigned to a cluster if the value is 2-4 in the allcell_types table. 

#### Example Usage 

```bash
mkdir -p ExampleOut/feature_assign_2_3 
./feature_assigner ExampleIn ExampleIn/tree.txt 2 3 ExampleOut
```

## Compile 
Use Cmake to compile 

cmake --build ArbOrder/cmake-build-debug --target feature_assigner 
