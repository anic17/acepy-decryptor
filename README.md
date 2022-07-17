# Acepy Ransomware Decryptor

This small tool is used to decrypt files encrypted by the Acepy Ransomware, which used XOR and a hard-coded key.

As it's a relatively weak encryption, it's pretty simple to make a decryptor for it. The code used is basically Acepy Ransomware but with functions reversed in a way so it decrypts the files instead of encrypting them.


# Usage

`acepydecryptor [folder to decrypt]`

If the first argument isn't passed, the decryptor will prompt the user for the folder which contains encrypted files with extension `.acepy`

# Demonstration

This is a folder which had been encrypted by Acepy Ransomware. It is just simple as running the program and entering the path of an encrypted folder.
If the folder doesn't contain any file with `.acepy` extension it will automatically quit


![imatge](https://user-images.githubusercontent.com/58483910/179411161-a02da8e9-819f-4751-89a6-b73631b0af36.png)
##### Screenshot showing the decryption of a folder



# Is it safe?

Yes, this program is 100% safe and it helps in the removal of infections by Acepy Ransomware.

**Copyright &copy; 2022 anic17 Software**
