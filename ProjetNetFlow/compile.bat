@echo off
REM Compilation du projet NetFlow
gcc -Isrc -Wall -g src/main.c src/graphe.c src/dijkstra.c src/securite.c src/backtracking.c src/utils.c src/liste_chainee.c src/simulation.c -o NetFlow_Sim.exe -lm

if %errorlevel% equ 0 (
    echo.
    echo [OK] Compilation reussie!
    echo.
    pause
) else (
    echo.
    echo [ERREUR] Echec de la compilation.
    echo.
    pause
)
