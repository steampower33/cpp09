CNT=5
LIMIT=8
INDEX=0
ITER=100
SUM=0
NUM_OVER=0
MAX=$LIMIT
ARG=`jot -r $CNT 1 2147483647 | tr "\n" " "`
NUM=`./PmergeMe $ARG | grep count | awk '{print $2}'`
while [ $INDEX -lt $ITER ]
do
echo "$INDEX    :   $NUM `./PmergeMe $ARG | grep count | awk '{print $2}'`"
if [ $NUM -ge $LIMIT ]; then
NUM_OVER=`expr $NUM_OVER + 1`
fi
if [ $NUM -gt $MAX ]; then
MAX=$NUM
fi
SUM=`expr $SUM + $NUM`
ARG=`jot -r $CNT 1 2147483647 | tr "\n" " "`
NUM=`./PmergeMe $ARG | grep count | awk '{print $2}'`
INDEX=`expr $INDEX + 1`
done
echo "$NUM_OVER overred operations for $ITER times (Average: `expr $SUM / $ITER`)"
# if [ $NUM_OVER -gt 0 ]; then
# echo "Biggest operation number : $MAX"
# echo "fix it!!!!!!!!!!"
# fi