read rtl tests/a2.v
blast ntk
bsetorder -file
bconstruct -all
print ntk -p
