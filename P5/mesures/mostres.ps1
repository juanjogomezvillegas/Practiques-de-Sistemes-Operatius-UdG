# Programa realitzat per Juan Jose Gomez Villegas (u1987338)

$fcopiats = 0;
$directoris = Get-ChildItem -Path . | Where-Object -Property Mode -EQ "d-----" # obté els directoris de la carpeta mostres
$nomsDir = $directoris | Select-Object -Property Name # Obté els noms dels directoris

if ($args.Length -eq 0) { # Si no s'ha entrat cap parametre
    foreach ($i in $nomsDir) {
        $fitxers = Get-ChildItem -Path $i.Name | Select-Object -Property Name

        foreach ($j in $fitxers) {
            $desti = $j.Name.Substring(0, 1) + "_" + $i.Name + ".out"
            $origen = $i.Name + "/" + $j.Name

            cp $origen $desti

            $fcopiats = $fcopiats + 1
        }
    }
} else { # Si s'ha entrat almenys un parametre
    //
}

Write-Output "S'han copiat $fcopiats fitxers de mostres." # Finalment, mostra el nombre de fitxers copiats
