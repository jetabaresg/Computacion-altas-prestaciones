# Ejercicios de programación paralela con MPI

Ejemplos de MPI en C y C++ para practicar compilación y ejecución distribuida:

- `material_s1/Ejercicio1.cpp`: saludo desde cada proceso MPI.
- `material_s1/esfera.c`: estimación Monte Carlo del volumen de una esfera.
- `material_s1/maestro-esclavo.c`: búsqueda distribuida de números primos.

## Requisitos

- Windows 10/11.
- [MSYS2](https://www.msys2.org/) con `mpicc.exe` y `mpic++.exe` disponibles, normalmente en `C:\msys64\ucrt64\bin`.
- Microsoft MPI, incluyendo `mpiexec.exe`.
- PowerShell 5.1 o posterior.

El script detecta el compilador correcto según la extensión: `mpicc` para `.c` y `mpic++` para `.cpp`, `.cc` y `.cxx`. Si MPI está instalado en otra ruta, define la variable `MSYS2_MPI_BIN`.

## Compilar y ejecutar desde PowerShell

Desde la raíz del proyecto:

```powershell
.\.vscode\build-mpi.ps1 -Source .\material_s1\Ejercicio1.cpp -Output .\material_s1\Ejercicio1.exe
& "C:\Program Files\Microsoft MPI\Bin\mpiexec.exe" -n 4 .\material_s1\Ejercicio1.exe

.\.vscode\build-mpi.ps1 -Source .\material_s1\esfera.c -Output .\material_s1\esfera.exe
& "C:\Program Files\Microsoft MPI\Bin\mpiexec.exe" -n 4 .\material_s1\esfera.exe 2 1000000

.\.vscode\build-mpi.ps1 -Source .\material_s1\maestro-esclavo.c -Output .\material_s1\maestro-esclavo.exe
& "C:\Program Files\Microsoft MPI\Bin\mpiexec.exe" -n 4 .\material_s1\maestro-esclavo.exe 5
```

También se puede usar `compilar.sh` desde Bash/MSYS2:

```bash
./compilar.sh material_s1/Ejercicio1.cpp material_s1/Ejercicio1.exe
```

En VS Code, abre el archivo fuente y ejecuta **Terminal > Run Build Task** para compilarlo. La tarea **Run MPI (4 processes)** compila y ejecuta el archivo activo.

## Parámetros

- `esfera.c`: `radio` obligatorio y `max_puntos` opcional. Ambos deben ser mayores que cero.
- `maestro-esclavo.c`: cantidad de primos opcional; por defecto busca uno. Requiere al menos 2 procesos MPI.
- `Ejercicio1.cpp`: no necesita parámetros.

## Firma opcional de ejecutables

La firma Authenticode no es necesaria para compilar ni ejecutar los ejemplos y no se guarda ningún certificado en el repositorio. Para firmar localmente, configura el pulgar del certificado en una variable de entorno y pasa `-Sign`:

```powershell
$env:MPI_SIGNING_CERT_THUMBPRINT = "TU_HUella_DIGITAL"
.\.vscode\build-mpi.ps1 -Source .\material_s1\Ejercicio1.cpp -Sign
```

No commits certificados, claves privadas, tokens, `.env` ni ejecutables. El `.gitignore` ya excluye estos archivos y los artefactos generados.

## Solución de problemas

- **No se encuentra `mpicc.exe`**: instala el paquete MPI de MSYS2 o define `MSYS2_MPI_BIN` con la carpeta que contiene `mpicc.exe` y `mpic++.exe`.
- **No se encuentra `mpiexec.exe`**: instala Microsoft MPI y actualiza la ruta de la tarea en `.vscode/tasks.json` si está en otra ubicación.
- **El programa no arranca**: comprueba que el ejecutable fue compilado y que el número de procesos (`-n`) es válido para el ejemplo.
