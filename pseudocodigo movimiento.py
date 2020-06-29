def SwapHomes(S, ti, tj):
    pass

def SwapMatches(S, ti, tj, rk):
    pass

def SwapMatchRounds(S, ri, rj, tk):
    pass

NTeams = 4
Nrounds = NTeams*2 - 2
S = []

##########################################

for ti in range(NTeams):
    for tj in range(ti+1, NTeams):
        SwapHomesResult = SwapHomes(S, ti, tj)
        # select the best
        for ri in range(Nrounds):
            SwapMatchesResult = SwapMatches(S, ti, tj, ri)
            # compare SwapHomesResult with SwapMatchesResul
            for rj in range(ri+1, Nrounds):
                print(ri, rj, tj)
                SwapMatchRoundsResult = SwapMatchRounds(S, ri, rj, ti)
                # compare the best from before with SwapMatchRoundsResult

##########################################

for ti in range(NTeams):
    for tj in range(ti+1, NTeams):
        SwapHomesResult = SwapHomes(S, ti, tj)
        for ri in range(Nrounds):
            SwapMatchesResult = SwapMatches(S, ti, tj, ri)
            # compare SwapHomesResult with SwapMatchesResul
for ri in range(Nrounds):
    for rj in range(ri+1, Nrounds):
        for ti in range(NTeams):
            print(ri, rj, ti)
            SwapMatchRoundsResult = SwapMatchRounds(S, ri, rj, ti)
            # compare the best from before with SwapMatchRoundsResult
