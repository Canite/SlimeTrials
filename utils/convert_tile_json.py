#!/usr/bin/env python

import sys
import os
import json
import argparse


def main(args):
    tile_data = []
    map_width = 0
    map_height = 0
    spawn_data = {}
    output_filename = os.path.basename(args.output)
    with open(args.input, 'r') as json_file:
        j = json.load(json_file)
        if "layers" not in j:
            print("Tile data missing layers, invalid file")
            sys.exit()

        layers = j["layers"]
        if (len(layers) == 0):
            print("Tile data layers empty, invalid file")
            sys.exit()

        tile_data = layers[0]["data"]
        map_width = layers[0]["width"]
        map_height = layers[0]["height"]

        if (len(layers) > 1):
            if (layers[1]["name"] == "spawn"):
                spawn_data = layers[1]

    for i in range(len(tile_data)):
        if tile_data[i] > 0:
            tile_data[i] -= 1

    with open(args.output + ".c", "w+") as out_c_file:
        out_c_file.write(f"/*\n\n Auto-generated from Tiled {output_filename}\n\n*/\n\n")
        if tile_data:
            out_c_file.write(f"const unsigned char {output_filename}_tiles[] = " + "\n" + "{" + "\n")
            tile_data_strs = ["    "]
            line_num = 0
            for i, tile_idx in enumerate(tile_data):
                tile_data_strs[line_num] += f"0x{tile_idx:02x},"
                if (i != 0 and (i + 1) % 16 == 0 and i != len(tile_data) - 1):
                    tile_data_strs[line_num] += "\n"
                    line_num += 1
                    tile_data_strs.append("    ")

            tile_data_strs[line_num] = tile_data_strs[line_num][:-1] + "\n"
            out_c_file.writelines(tile_data_strs)
            out_c_file.write("};\n")

    with open(args.output + ".h", "w+") as out_h_file:
        out_h_file.write(f"/*\n\n Auto-generated from Tiled {output_filename}\n\n*/\n\n")
        if tile_data:
            out_h_file.write(f"#ifndef _{output_filename.upper()}_H\n")
            out_h_file.write(f"#define _{output_filename.upper()}_H\n\n")
            out_h_file.write(f"#define {output_filename}_tile_width  {map_width}\n")
            out_h_file.write(f"#define {output_filename}_tile_height  {map_height}\n")

            if (spawn_data):
                spawn_x = spawn_data["objects"][0]["x"] + 16
                spawn_y = spawn_data["objects"][0]["y"] + 16
                out_h_file.write(f"#define {output_filename}_spawn_x  {spawn_x}\n")
                out_h_file.write(f"#define {output_filename}_spawn_y  {spawn_y}\n")

            out_h_file.write(f"extern const unsigned char {output_filename}_tiles[];\n")
            out_h_file.write("\n#endif\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", required=True)
    parser.add_argument("-o", "--output", required=True)

    main(parser.parse_args())
