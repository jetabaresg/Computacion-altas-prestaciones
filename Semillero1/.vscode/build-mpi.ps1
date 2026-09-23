param(
    [string]$Source,
    [string]$Output,
    [switch]$Sign
)

$mpicDir = if ($env:MSYS2_MPI_BIN) { $env:MSYS2_MPI_BIN } else { "C:\msys64\ucrt64\bin" }
$mpicc = Join-Path $mpicDir "mpicc.exe"
$mpicpp = Join-Path $mpicDir "mpic++.exe"

if (-not $Source -or -not (Test-Path -LiteralPath $Source)) {
    Write-Error "Indica un archivo fuente existente con -Source."
    exit 2
}

if (-not $Output) {
    $Output = [IO.Path]::ChangeExtension($Source, ".exe")
}

if ($Source -match '\.(cpp|cc|cxx)$') {
    $compiler = $mpicpp
} elseif ($Source -match '\.c$') {
    $compiler = $mpicc
} else {
    Write-Error "Extensión no compatible: $Source (usa .c, .cpp, .cc o .cxx)."
    exit 2
}

if (-not (Test-Path -LiteralPath $compiler)) {
    Write-Error "No se encontró $compiler. Instala MSYS2 MPI o define MSYS2_MPI_BIN."
    exit 2
}

$outputDirectory = Split-Path -Parent $Output
if ($outputDirectory -and -not (Test-Path -LiteralPath $outputDirectory)) {
    New-Item -ItemType Directory -Path $outputDirectory -Force | Out-Null
}

Write-Host "Compilando con: $compiler"
& $compiler -Wall -Wextra -O2 -o $Output $Source
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

if ($Sign) {
    $certThumb = $env:MPI_SIGNING_CERT_THUMBPRINT
    if (-not $certThumb) {
        Write-Error "Para firmar, define MPI_SIGNING_CERT_THUMBPRINT y usa -Sign."
        exit 2
    }

    $cert = Get-ChildItem Cert:\CurrentUser\My | Where-Object { $_.Thumbprint -eq $certThumb }
    if (-not $cert) {
        Write-Error "No se encontró el certificado indicado en el almacén del usuario."
        exit 2
    }

    $sign = Set-AuthenticodeSignature -FilePath $Output -Certificate $cert
    if ($sign.Status -ne "Valid") {
        Write-Error "La firma no es válida: $($sign.StatusMessage)"
        exit 1
    }
    Write-Host "Firmado correctamente: $Output"
}

Write-Host "Listo: $Output"
exit 0