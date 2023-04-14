#!/usr/bin/env python3

import re
import subprocess

from pprint import pprint


PROB_SIZES = (1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000)
THREAD_CTS = (1, 2, 4, 8, 16, 32, 64, 128)
PROGS = (
    'prime', 'prime-omp', 'prime-omp2', 'prime-par1',
    'prime-par2', 'prime-par3'
)

def main():
    times_by_prob_size = []
    for prob_size in PROB_SIZES:
        times_by_thread_ct = []
        header = ','.join(['Threads'] + list(PROGS))
        times_by_thread_ct.append(header)

        for thread_ct in THREAD_CTS:
            times_by_prog = [str(thread_ct)]
            for prog in PROGS:
                result = subprocess.run(('./' + prog, str(prob_size),
                                        str(thread_ct)),
                                        text=True, capture_output=True)
                time = re.search('([0-9.]+) ms', result.stdout)
                if time is not None:
                    time = time[1]
                times_by_prog.append(str(time))
            times_by_thread_ct.append(','.join(times_by_prog))

        times_by_prob_size.append(times_by_thread_ct)

    for idx, table in enumerate(times_by_prob_size):
        print(f'Problem size: {PROB_SIZES[idx]}')
        print('\n'.join(table))
        print()


if __name__ == '__main__':
    main()
