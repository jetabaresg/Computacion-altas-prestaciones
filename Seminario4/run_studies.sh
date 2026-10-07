export OMP_PROC_BIND=close
export OMP_PLACES=cores

echo "2: Estudio tamaño de problema con hilos máximos"
echo "n b threads time error"
for n in 500 1000 2000 4000; do
    for b in 16 32 64 128 256; do
        if [ $b -gt $n ]; then continue; fi
        ./cholesky_par $n $b 2>&1 | tail -1
    done
done

echo ""
echo "3: Estudio variando número de hilos (n=2000, b=64)"
echo "n b threads time error"
for t in 1 2 4 8 16 32; do
    ./cholesky_par 2000 64 $t 2>&1 | tail -1
done

echo ""
echo "4: Estudio políticas de planificación (n=2000, b=64, 8 hilos)"
echo "schedule time error"
for sched in static dynamic guided; do
    for chunk in 1 4 16; do
        export OMP_SCHEDULE="$sched,$chunk"
        ./cholesky_par 2000 64 8 2>&1 | tail -1
    done
done