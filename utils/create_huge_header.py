#!/usr/bin/env python

import os
import argparse


def main(args):
    huge_song_name = os.path.splitext(os.path.basename(args.input))[0]
    output_filename = os.path.splitext(args.input)[0] + ".h"

    with open(output_filename, 'w+') as huge_header:
        huge_header.write(f"#ifndef _{huge_song_name.upper()}_H\n")
        huge_header.write(f"#define _{huge_song_name.upper()}_H\n\n")
        huge_header.write('#include "../include/hUGEDriver.h"\n\n')
        huge_header.write(f"extern const hUGESong_t {huge_song_name};\n")
        huge_header.write(f"BANKREF_EXTERN({huge_song_name})\n\n")
        huge_header.write("#endif\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", required=True)

    main(parser.parse_args())
