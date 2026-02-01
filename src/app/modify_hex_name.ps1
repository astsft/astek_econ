$commit = git show -s --pretty=format:"%h" HEAD
$commit

$exec_path = Get-Location
#"Execute location $exec_path"

#$exec_path_array = $exec_path.Split("\")
#$exec_path_array

$Inputstring ="$exec_path"
"Exec path $exec_path"	
$CharArray =$InputString.Split("\")

$script_path = $PSScriptRoot
"Current location $script_path"

#$app_name = Split-path $script_path -Leaf
$workspace_name = $CharArray[-5]
"Current workspace $workspace_name"	

$CharArray2 = $CharArray[-3].Split(".")
$app_name = $CharArray2[0]
"Current app $app_name"	

$hw_name = $CharArray2[-1].Substring(2)
"Current HW $hw_name"
	
Remove-Item $script_path\..\..\hex\$app_name\$hw_name\$app_name.*$hw_name.*.hex		
	
$filename = Get-ChildItem $script_path\..\..\hex\$app_name\$hw_name\$app_name* -Name
"Old filename $filename"

$idx = $filename.IndexOf("hex")
#$idx

$filename1 = $filename.Remove($idx)
#$filename1

$version_path = "$script_path\$workspace_name\version\version.c"
#$version_path

foreach($line in Get-Content $version_path) {
		$version_str = $line
}
#$version_str
$version_idx = $version_str.IndexOf('"')
$datatime = $version_str.Substring($version_str.IndexOf('"') + 1, 13)
#$datatime

#$datatime = "$(Get-Date -Format "MMM-dd-yyyy_HH-mm-ss")"
#$datatime

$newfilename = "$filename1" + "$datatime" + "_$commit.hex"
"New filename $newfilename"

Rename-Item $script_path\..\..\hex\$app_name\$hw_name\$filename -NewName $newfilename
#Copy-Item -Path $script_path\..\..\iar\project\hex\$newfilename -Destination $script_path\..\..\
if ($hw_name -eq "2331_rev3")
{
	$preconfig_path = "$script_path\preconfig.h"
	$preconfig_path

	$SEL = Select-String -Path $preconfig_path -Pattern "ASBACK_HW2353REV3"	

	if ($SEL -ne $null)
	{
	    $asback_hw="ASBACK_HW2353REV5"
		$asback_hw
	}	

	$SEL = Select-String -Path $preconfig_path -Pattern "ASBACK_HW2353REV5"	

	if ($SEL -ne $null)
	{
	    $asback_hw="ASBACK_HW2353REV5"
		$asback_hw
	}	

	$SEL = Select-String -Path $preconfig_path -Pattern "EKON_2025"

	if ($SEL -ne $null)
	{
	    $ekon_2025="EKON_2025\"
		$ekon_2025
	}
	else
	{
		$ekon_2025=""
	}
	
	$SEL = Select-String -Path $preconfig_path -Pattern "USE_VALIDATION"

	if ($SEL -ne $null)
	{
	    $use_validation="USE_VALIDATION\"
		$use_validation
	}
	else
	{
		$use_validation=""
	}	

	Remove-Item -Path $script_path\..\..\hex\$app_name\$hw_name\$asback_hw\$ekon_2025\$use_validation*.*
	Move-Item -Path $script_path\..\..\hex\$app_name\$hw_name\$newfilename -Destination $script_path\..\..\hex\$app_name\$hw_name\$asback_hw\$ekon_2025\$use_validation$newfilename
	"Move fw to $script_path\..\..\hex\$app_name\$hw_name\$asback_hw\$ekon_2025$newfilename"	
}