﻿# Programa realitzat per Juan Jose Gomez Villegas (u1987338)

# Pre: $r és la ruta on són els directoris
# Post: Retorna un array de noms de directoris
Function Directoris ($r) {
    Get-ChildItem -Path $r | Where-Object -Property Mode -EQ "d-----" | Select-Object -Property Name
}

# Pre: $d és el directori on són els fitxers
# Post: Retorna un array de noms de fitxers
Function Fitxers ($d) {
    Get-ChildItem -Path $d | Where-Object -Property Mode -EQ "-a----" | Select-Object -Property Name
}

$fcopiats = 0;
$errors = 0;
$directoris = Directoris('.') # obté els directoris de la carpeta mostres

if ($args.Length -eq 0) { # Si no s'ha entrat cap parametre
    foreach ($i in $directoris) {
        $fitxers = Fitxers($i.Name)

        foreach ($j in $fitxers) {
            $origen = $i.Name + "/" + $j.Name
            $desti = $j.Name.Substring(0, 1) + "_" + $i.Name + ".out"
            
            cp $origen $desti

            $fcopiats = $fcopiats + 1
        }
    }
} else { # Si s'ha entrat almenys un parametre
    foreach ($i in $args) { # Abans de tot, valida els parametres un a un, fent servir una expressio regular
        if (!($i -match '^[a-z]$')) {
            $errors = $errors + 1;
        }
    }

    if ($errors -gt 0) { # I si troba algun error, mostra un missatge
        Write-Output "Error: Els parametres no son correctes"
        exit
    } else { # Si els parametres son correctes
        foreach ($a in $args) {
            foreach ($i in $directoris) {
                $fitxers = Fitxers($i.Name)

                foreach ($j in $fitxers) {
                    if ($a.ToLower() -eq $j.Name.Substring(0, 1)) {
                        $origen = $i.Name + "/" + $j.Name
                        $desti = $j.Name.Substring(0, 1) + "_" + $i.Name + ".out"
            
                        cp $origen $desti

                        $fcopiats = $fcopiats + 1
                    }
                }
            }
        }
    }
}

Write-Output "S'han copiat $fcopiats fitxers de mostres." # Finalment, mostra el nombre de fitxers copiats