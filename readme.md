# Projet The train
Par Alina & Yasmine

## Pour lancer le programme : (depuis le dossier bin)
- Sous Windows:
./TD_the_train_ex01.exe ../TD_the_train/circuit.json

- Sous Linux:
./TD_the_train_ex01 ../TD_the_train/circuit.json

## Pour rebuild : (depuis le dossier build)
cmake --build .

## Tout en 1 ligne:
- Sous Windows:
cd ../build ; cmake --build . ; cd ../bin ; ./TD_the_train_ex01.exe ../TD_the_train/circuit.json

-Sous Linux :
cd ../build ; cmake --build . ; cd ../bin ; ./TD_the_train_ex01 ../TD_the_train/circuit.json

# Touches utiles:

- Z/Q/S/D: se déplacer
- Souris : orienter la caméra
- L : wireframe
- P : affichage normal (polygones remplis)
- F : activer / désactiver les lumières
- N : mode jour / nuit
- Échap : quitter l'application