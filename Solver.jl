import Pkg
Pkg.add(["JuMP", "HiGHS", "Format"])

using JuMP
using HiGHS
using Format

function solve_min_distance_schedule(N, P; time_limit=nothing, UB=nothing)

    totalP = sum(P)

    model = Model(HiGHS.Optimizer)

    #
    # Parâmetros do solver
    #

    if time_limit !== nothing
        set_time_limit_sec(model, time_limit * 60.0)
    end

    # Para de procurar quando achar uma solução que seja 1% pior que a melhor teórica
    set_optimizer_attribute(model, "mip_rel_gap", 0.01)

    #
    # Variáveis
    #

    @variable(model, D >= 0)
    if UB == nothing
        @variable(model, S[1:N] >= 0)
    else
        @variable(model, 0 <= S[i=1:N] <= UB)
    end

    # Solução inicial

    start = zeros(Float64, N) # usado para setar inicialmente as booleanas

    order = sortperm(P)
    current = 0

    for i in order
        start[i] = current
        set_start_value(S[i], current)
        current += maximum(P)
    end


    if UB == nothing
        UB = current
    end


    # b(i,j) só existe para i<j
    @variable(model, b[i=1:N-1, j=i+1:N], Bin)

    # Valores binários iniciais

    for i in 1:N
        for j in i+1:N
            minP = min(P[i], P[j])

            if start[j] >= start[i] + minP
                set_start_value(b[i,j], 1)
            else
                set_start_value(b[i,j], 0)
            end
        end
    end

    #
    # Objetivo
    #

    @objective(model, Min, D)

    #
    # Fixação de origem
    #

    @constraint(model, S[1] == 0)

    #
    # Makespan
    #

    @constraint(model, [i=1:N], D >= S[i] + P[i])

    #
    # Limite superior (conhecido anteriormente)
    #

    if UB != nothing
        @constraint(model, D <= UB)
    end

    #
    # Restrições de distância mínima (Big-M forte)
    #

    for i in 1:N-1
        for j in i+1:N

            minP = min(P[i], P[j])

            # Big-M específico do par
            Mij = totalP - minP

            @constraint(model,
                S[i] >= S[j] + minP - Mij * (1 - b[i,j])
            )

            @constraint(model,
                S[j] >= S[i] + minP - Mij * b[i,j]
            )
        end
    end

    optimize!(model)

    println("\nStatus: ", termination_status(model))
    println()

    if has_values(model)

        println("Makespan = ", value(D))
        println()

        println("Tempos de início:")
        for i in 1:N
            println(format("Tarefa {:2d}: {:8.2f}", i, value(S[i])))
        end

    else
        println("Nenhuma solução encontrada.")
    end

end

#
# Leitura do arquivo
#

function read_data_from_file(filename)
    open(filename, "r") do io
        N = parse(Int, readline(io))
        P = [parse(Int, line) for line in eachline(io)]
        return N, P
    end
end

#
# Leitura CLI e execução
#

if length(ARGS) < 1
    println("Uso: julia script.jl <arquivo.dat> [tempo_limite_minutos] [Upper Bound conhecido]")
    println("Opicional --threads=auto --optimize=3")
    exit()
end

filename = ARGS[1]

time_limit = nothing
UB = nothing

for arg in ARGS[2:end]
    key, value = split(arg, "=")
    
    if key == "--time_limit"
       global time_limit = parse(Int, value)
    elseif key == "--UB"
       global UB = parse(Int, value)
    end
end

N, P = read_data_from_file(filename)

solve_min_distance_schedule(N, P; time_limit=time_limit, UB=UB)
