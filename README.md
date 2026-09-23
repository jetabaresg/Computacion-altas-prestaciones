# CNA — Computación de Altas Prestaciones (UPV)

Prácticas, semilleros y apuntes de la asignatura de **Computación de Altas Prestaciones** del curso 2026–2027: programación paralela con **MPI**, **OpenMP** y ejecución en el clúster **Kahan (UPV)**.

## Estructura

| Carpeta | Contenido |
|---|---|
| `Practicas/` | Prácticas de la asignatura |
| `Semillero1/` | Introducción a MPI: ejemplos en C/C++ y guía de compilación |
| `Semillero2/` | OpenMP y clúster Kahan: ejercicios knapsack, matrix product y scripts Slurm |
| `Semillero3/` | Ejercicios del seminario 3 |

## Requisitos generales

- Windows 10/11 (o Linux/WSL)
- [MSYS2](https://www.msys2.org/) con `mpicc` / `mpic++` para MPI
- Microsoft MPI (`mpiexec`)
- Compilador con soporte OpenMP (GCC / MSVC)
- Acceso al clúster Kahan para los trabajos Slurm (red UPV o Polilabs)

## Uso rápido

Cada semillero tiene su propio `README.md` con instrucciones detalladas. Ejemplo con MPI desde la raíz:

```powershell
.\Semillero1\.vscode\build-mpi.ps1 -Source .\Semillero1\material_s1\esfera.c -Output .\Semillero1\material_s1\esfera.exe
& "C:\Program Files\Microsoft MPI\Bin\mpiexec.exe" -n 4 .\Semillero1\material_s1\esfera.exe 2 1000000
```

Ver también `Semillero1/compilar.sh` para Bash/MSYS2 y las tareas de VS Code.

## Notas

- Los **secretos** (`.env`, claves, tokens), la **guía local del clúster** y los **ejecutables** (`.exe`) no se versionan: están excluidos en `.gitignore`.
- Los **PDF** de teoría y seminarios no se incluyen en este repositorio público por derechos de la UPV.
