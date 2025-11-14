## Runs without assignment task
mkdir ExampleOut
./arbAssigner ExampleIn ExampleIn/config_k5.txt ExampleIn/tree.txt 5 C8 0 ExampleOut

## Run with assignment task. This writes gene assignments (gene -> cluster)
# based on the if they are expressed (have value 4-5) in the cluster.
mkdir ExampleOut/feature_assign_4_5
./arbAssigner ExampleIn ExampleIn/config_k5.txt ExampleIn/tree.txt 4 5 C8 0 ExampleOut

## Runs without assignment task, fixes unaasigned genes.
# gene can be unassigned if there expression is an outlier. In case we just assign to cluster with closest mean.
# Note this ignores the guassian estimated variance but in practice this effect should be minimal
./arbAssigner ExampleIn ExampleIn/config_k5.txt ExampleIn/tree.txt 5 C8 0 ExampleOut

## Run with assignment task and fixes unassigned genes.  This writes gene assignments (gene -> cluster),
# based on the if they are expressed (have value 4-5) in the cluster.
# gene can be unassigned if there expression is an outlier. In case we just assign to cluster with closest mean.
# Note this ignores the guassian estimated variance but in practice this effect should be minimal
mkdir ExampleOut/feature_assign_4_5
./arbAssigner ExampleIn ExampleIn/config_k5.txt ExampleIn/tree.txt 4 5 C8 0 ExampleOut