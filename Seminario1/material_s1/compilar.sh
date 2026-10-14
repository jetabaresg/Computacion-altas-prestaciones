#!/bin/bash
# Uso: ./compilar.sh <archivo.c|cpp> [salida.exe]
# Compila con mpicc/mpic++. La firma es opcional: MPI_SIGNING_CERT_THUMBPRINT=... ./compilar.sh ... --sign

if [ -z "$1" ]; then
  echo "Uso: $0 <archivo.fuente> [salida.exe]"
  exit 1
fi

SRC="$1"
SIGN=""

if [ -z "$2" ]; then
  BASE="${SRC%.*}"
  OUT="$BASE.exe"
else
  OUT="$2"
fi

if [ "${3:-}" = "--sign" ]; then
  SIGN="-Sign"
fi

powershell.exe -NoProfile -ExecutionPolicy Bypass -File "$(dirname "$0")/.vscode/build-mpi.ps1" -Source "$SRC" -Output "$OUT" $SIGN