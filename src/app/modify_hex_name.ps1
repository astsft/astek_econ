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
#"Current location $script_path"

#$app_name = Split-path $script_path -Leaf
$app_name = $CharArray[-5]
"Current app $app_name"	

$hw_name = $CharArray[-3].Substring($CharArray[-3].IndexOf('.') + 3, 4)
"Current HW $hw_name"
	
Remove-Item $script_path\..\..\hex\$hw_name\$app_name*_*.hex	
	
$filename = Get-ChildItem $script_path\..\..\hex\$hw_name\$app_name* -Name
"Old filename $filename"

$idx = $filename.IndexOf("hex")
#$idx

$filename1 = $filename.Remove($idx)
#$filename1

$version_path = "$script_path\$app_name\version\version.c"
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

Rename-Item $script_path\..\..\hex\$hw_name\$filename -NewName $newfilename
#Copy-Item -Path $script_path\..\..\iar\project\hex\$newfilename -Destination $script_path\..\..\
