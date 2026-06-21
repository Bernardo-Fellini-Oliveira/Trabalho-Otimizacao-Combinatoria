# ==============================
# Configuracao
# ==============================
$exe = ".\heuristica.exe"

# Parametros fixos
$p2_default = 3
$p3_default = 100
$p4_default = 50
$p6 = 0
$p7 = 100
$p8 = 789245

# ==============================
# Funcoes estatisticas
# ==============================
function Media($valores) {
    return ($valores | Measure-Object -Average).Average
}

function DesvioPadrao($valores) {
    $media = Media $valores
    $soma = 0

    foreach ($v in $valores) {
        $soma += [math]::Pow(($v - $media), 2)
    }

    return [math]::Sqrt($soma / $valores.Count)
}

# ==============================
# Execucao + coleta
# ==============================
function Rodar-Conjunto($p2, $p3, $p4, $pastaResultados, $pastaInstancias) {

    $p5 = $p3 - $p4

    $makespans = @()
    $tempos = @()

    $total = 1
    if ($pastaResultados -eq "testes\$size\resultados_iter") {
        $total = 3
    } else {
        $total = 100
    }

    for ($x = 1; $x -le $total; $x++) {

        $saida = "$pastaResultados\resultados_$x.txt"
        $inst = "$pastaInstancias\inst_$x.txt"

        $start = Get-Date

        & $exe $saida $p2 $p3 $p4 $p5 $p6 $p7 $p8 $inst | Out-Null

        $end = Get-Date
        $tempoExec = ($end - $start).TotalSeconds

        $linhas = Get-Content $saida
        $makespan = [double]$linhas[-1]

        $makespans += $makespan
        $tempos += $tempoExec
    }

    return @{
        media_makespan = Media $makespans
        dp_makespan = DesvioPadrao $makespans
        media_tempo = Media $tempos
        dp_tempo = DesvioPadrao $tempos
    }
}

# ==============================
# Escrita padrao
# ==============================
function Salvar-Resultado($arquivo, $res) {

@"
media_makespan: $($res.media_makespan)
dp_makespan: $($res.dp_makespan)
media_tempo: $($res.media_tempo)
dp_tempo: $($res.dp_tempo)
"@ | Out-File $arquivo
}

$inst_sizes = @(50, 100, 1000)
foreach ($size in $inst_sizes) {

    $pastaInstancias = "testes\$size\instancias"
    $pastaResultados = "testes\$size\resultados_iter"
    $pastaMedias = "testes\$size\medias"

    New-Item -ItemType Directory -Force -Path $pastaMedias | Out-Null

    # ==============================
    # Variando p2
    # ==============================
    $p2_values = @(1, 3, 5, 10, 30)

    foreach ($p2 in $p2_values) {

        $res = Rodar-Conjunto $p2 $p3_default $p4_default $pastaResultados $pastaInstancias

        $arquivo = "$pastaMedias\p2_$p2.txt"
        Salvar-Resultado $arquivo $res

        Write-Host "p2=$p2 -> makespan=$($res.media_makespan) tempo=$($res.media_tempo)"
    }

    # ==============================
    # Variando p3 (p4 = p3/2)
    # ==============================
    for ($p3 = 20; $p3 -le 200; $p3 += 20) {

        $p4 = [int]($p3 / 2)

        $res = Rodar-Conjunto $p2_default $p3 $p4 $pastaResultados $pastaInstancias

        $arquivo = "$pastaMedias\p3_$p3.txt"
        Salvar-Resultado $arquivo $res

        Write-Host "p3=$p3 -> makespan=$($res.media_makespan) tempo=$($res.media_tempo)"
    }

    # ==============================
    # Variando p4
    # ==============================
    for ($p4 = 10; $p4 -le 100; $p4 += 10) {

        $res = Rodar-Conjunto $p2_default $p3_default $p4 $pastaResultados $pastaInstancias

        $arquivo = "$pastaMedias\p4_$p4.txt"
        Salvar-Resultado $arquivo $res

        Write-Host "p4=$p4 -> makespan=$($res.media_makespan) tempo=$($res.media_tempo)"
    }

}


Write-Host "==============================="
Write-Host "Experimentos finalizados!"
Write-Host "==============================="