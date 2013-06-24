#  Create ISO-8859-1 Code Page Description
#
#  Usage: mk8859.pl >ISO8859.txt


print "#  ISO-8859-1 Code Page\n";
print "#  Just maps every single-byte character (SBC) code onto the same Unicode value\n";

for ($i=0; $i<256; $i++) {
    printf "0x%02x\t0x%04x\n", $i, $i;
}
