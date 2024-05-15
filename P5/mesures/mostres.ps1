# Programa realitzat per Juan José Gómez Villegas (u1987338)

# Pre: cert
# Post: retorna la ruta des d'on executar l'script
Function entrarMesures () {
    if ((Get-ChildItem -Path . | Where-Object -Property Name -EQ mesures).Length -eq 0) { # si no troba cap directori mesures, retorna el directori actual . com a ruta
        # en aquest cas, se suposa que l'script mostres estarà a dins del directori mesures (com a la pràctica 1)
		$ruta = "."
    } else { # si troba un directori mesures, retorna el directori mesures com a ruta
        $ruta = "mesures"
    }

    $ruta
}

# Pre: $r és la ruta on són els directoris
# Post: retorna un array de noms de directoris
Function Directoris ($r) {
	if (Test-Path $r) { # si $r existeix, retorna només els directoris
		Get-ChildItem -Path $r | Where-Object -Property Mode -EQ "d-----" | Select-Object -Property Name
	} else { # si no, acaba
		exit
	}
}

# Pre: $d és el directori on són els fitxers
# Post: retorna un array de noms de fitxers
Function Fitxers ($d) {
    if (Test-Path $d) { # si $d existeix, retorna només els fitxers
		Get-ChildItem -Path $d | Where-Object -Property Mode -EQ "-a----" | Select-Object -Property Name
	} else { # si no, acaba
		exit
	}
}

$ruta = entrarMesures # estableix la ruta des d'on copiar els fitxers

$fcopiats = 0
$errors = 0
$directoris = Directoris($ruta) # obté els directoris que hi ha a $ruta

if ($args.Length -eq 0) { # si no s'ha entrat cap paràmetre
    foreach ($i in $directoris) { # va iterant per cada directori
        $fitxers = Fitxers($ruta + "/" + $i.Name) # obté tots els fitxers de $i

        foreach ($j in $fitxers) { # i va iterant per cada fitxer de $i, fent una còpia de $j amb el nom de $i afegit
			$origen = $ruta + "/" + $i.Name + "/" + $j.Name
            $desti = $j.Name.Substring(0, 1) + "_" + $i.Name + ".out"
            
            cp $origen $desti # còpia el fitxer $j

            $fcopiats = $fcopiats + 1 # i incrementa el nombre de fitxers copiats
        }
    }
} else { # si s'ha entrat almenys un paràmetre
    foreach ($i in $args) { # abans de tot, valida els paràmetres un a un, fent servir una expressió regular
        if (!($i -match '^[a-z]$')) { # expressió regular: són vàlids els paràmetres que siguin un sol caràcter d'inici a fi, en aquesta pràctica no comprova si està en minúscula, ja que existeix la funció lowercase()
            $errors = $errors + 1;
        }
    }

    if ($errors -gt 0) { # si troba algun error, mostra un missatge i acaba
        Write-Output "Error: Els parametres no son correctes"
        exit
    } else { # Si els paràmetres són correctes
        foreach ($a in $args) { # va iterant per cada paràmatre
            foreach ($i in $directoris) { # i va iterant per cada directori
                $fitxers = Fitxers($ruta + "/" + $i.Name) # obté tots els fitxers de $i

                foreach ($j in $fitxers) { # i va iterant per cada fitxer de $i, i comprovant que $j és igual a $a (en minúscula)
                    if ($a.ToLower() -eq $j.Name.Substring(0, 1)) {
						# si coincideixen, còpia $j amb el nom de $i afegit
                        $origen = $ruta + "/" + $i.Name + "/" + $j.Name
                        $desti = $j.Name.Substring(0, 1) + "_" + $i.Name + ".out"

                        cp $origen $desti # còpia el fitxer $j

                        $fcopiats = $fcopiats + 1 # i incrementa el nombre de fitxers copiats
                    }
                }
            }
        }
    }
}

Write-Output "S'han copiat $fcopiats fitxers de mostres." # finalment, mostra el nombre de fitxers copiats
