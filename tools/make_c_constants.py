# generate array that can be pasted into a c file

import itertools

cycle_length = 256

def getValues(upper_value, cycles):
    inc = cycles * 2
    value = 0
    values = []
    mult = 1
    for i in range(cycle_length):
        if value < 0:
            mult *= -1
            value *= -1
        elif value >= upper_value:
            mult *= -1
            value = upper_value - 1 - (value % upper_value)
        values.append(value)
        value += (inc * mult)
    return values

    
def grouper(iterable, n, fillvalue=None):
    "Collect data into fixed-length chunks or blocks"
    # grouper('ABCDEFG', 3, 'x') --> ABC DEF Gxx
    args = [iter(iterable)] * n
    return itertools.izip_longest(fillvalue=fillvalue, *args)


for row in grouper(getValues(256, 6), 16):
    print ', '.join(['{0: >3d}'.format(r) for r in row]) + ','


    
    