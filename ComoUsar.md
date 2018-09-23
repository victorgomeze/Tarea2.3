 Tarea2.3 Paralela

# Para compilar utilice :
# mpiCC "nombre_Archivo".cpp -o "nombreEjecutable"
# EJEMPLO : mpiCC T23.cpp -o T3
# 
# Para ejecutar utilice
# mpi run -np "Numero de Procesadores" ./"NombreEjecutable" "RutaArchivoCSV"
# EJEMPLO : mpirun -np 4 ./T3 /home/kototeus/Escritorio/utem.csv
#
# En caso de no saber la ruta del archivo CSV utilice
# readlink -f "nombre_archivo"
# EJEMPLO : readlink -f utem.csv
