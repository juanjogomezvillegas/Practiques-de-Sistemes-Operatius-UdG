# Practiques de Sistemes Operatius a la UdG

## P1: Programa amb shell que copia fitxers

Per executar-ho, abans cal fer.

```sh
$ cd P1; tar -xvf mesures; cd mesures
```
i una possible execució que cerca i copia tots els fitxers que es diguin *a*, *b* o *c*.

```sh
$ ./mostres a b c
```

[Per defecte si no se li passa cap paràmetre cerca i copia tots els fitxers]{style="color: blue"}

## P2: Programa amb shell que imita a la comanda find de unix

Els paràmetres d'aquest script són opcionals, si no es passa cap paràmetre mostra els fitxers del directori actual.

```sh
$ ./busca
```

I pot canviar de directori, i ordenar els fitxers per mida (per defecte s'ordenen de forma alfanùmerica).

També pot redirigir la sortida estàndard cap a un fitxer.

```sh
$ ./busca -d ~ -o mida -s sortida.txt
```

I es poden afegir filtres per *inode*, *nom del fitxer* i *permisos*.

```sh
$ ./busca -d ~ -o mida -s sortida.txt -n "sortida" -i 20 -p -rw------- 
```

## P3: Programa amb C que mostra l'estat d'un procés

Per executar l'script cal fer.

```sh
$ gcc -o proc proc.c
$ ps -ef | grep <nom d'usuari de unix>
```
[El grep es per cercar els processos als que es tinguin permisos]{style="color: blue"}

I un cop obtingut el pid del procés ja es pot fer.

```sh
$ ./proc <PID del procés>
```

L'script imita l'execució de la comanda.

```sh
$ cat /proc/<PID del procés>/status | grep Vm | sort -nrk2 >> sortida.txt
```

## P4: Programa amb C

## P5: Programa amb Powershell
