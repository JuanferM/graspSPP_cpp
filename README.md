# graspSPP_cpp
GRASP pour le Set Packing Problem (SPP)

*Commande pour compiler*:
```bash
make clean && make
```

*Commande pour lancer le programme*:
```bash
./bin/DM2
```

**ATTENTION:** le répertoire `instances` doit se trouver dans le même répertoire
d'où le programme est lancé. Si le programme est lancé depuis `bin` alors `instances`
doit être dans `bin`. Pour utiliser le makefile les répertoires `bin`, `include`,
`src` et `obj` doivent être présents.


### GLPK
Pour lancer GLPK sur les instances veuillez redéfinir USE_GLPK (macro dans le
fichier `src/main.cpp`) à true :
```c
#define USE_GLPK true
```
Sinon pour des descentes normales :
```c
#define USE_GLPK false
```

De plus, pour activer tous les messages de GLPK redéfinir VERBOSE_GLPK (macro dans
le fichier `src/main.cpp`) à true :
```c
#define VERBOSE_GLPK true
```
Sinon pour ne recevoir que les messages normaux :
```c
#define VERBOSE_GLPK false
```


### Paramètres du GRASP
#### Alpha
Pour modifier la valeur de alpha veuillez redéfinir ALPHA (macro dans le
fichier `src/main.cpp`) :
```c
#define ALPHA <x>
```
Où _x_ est un flottant entre 0 et 1.
#### Nombre d'itérations
Pour modifier le nombre d'itérations veuillez redéfinir NUM_ITER (macro dans le
fichier `src/main.cpp`) :
```c
#define NUM_ITER <x>
```
Où _x_ est un nombre entier strictement positif.
#### (Plus profonde) descente
Pour effectuer des améliorations par recherche locale de type plus profonde descente
veuillez redéfinir DEEPSEARCH (macro dans le fichier `src/main.cpp`) à true :
```c
#define DEEPSEARCH true
```
Sinon pour des descentes normales :
```c
#define DEEPSEARCH false
```

### Listes des instances qui ont été utilisées pour l'expérimentation
* didactic.dat
* pb_100rnd0100.dat
* pb_200rnd0100.dat
* pb_200rnd0300.dat
* pb_200rnd0900.dat
* pb_200rnd1500.dat
* pb_500rnd0100.dat
* pb_500rnd0700.dat
* pb_1000rnd0100.dat
* pb_1000rnd0500.dat
