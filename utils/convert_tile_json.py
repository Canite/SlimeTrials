#!/usr/bin/env python

import sys
import os
import json
import argparse


def main(args):
    tile_data = []
    tile_idx = 0
    col_data = []
    col_tile_idx = 0
    map_width = 0
    map_height = 0
    spawn_data = {}
    output_filename = os.path.splitext(os.path.basename(args.output))[0]
    with open(args.input, 'r') as json_file:
        j = json.load(json_file)
        if "layers" not in j:
            print("Tile data missing layers, invalid file")
            sys.exit()

        layers = j["layers"]
        if (len(layers) == 0):
            print("Tile data layers empty, invalid file")
            sys.exit()

        for layer in layers:
            layer_name = layer["name"]
            if (layer_name == "tiles"):
                tile_data = layer["data"]
                map_width = layer["width"]
                map_height = layer["height"]
            elif (layer_name == "collisions"):
                col_data = layer["data"]
            elif (layer_name == "spawn"):
                spawn_data = layer

        tilesets = j["tilesets"]
        for tileset in tilesets:
            if tileset["source"] == "background.tsj":
                col_tile_idx = int(tileset["firstgid"])
            else:
                tile_idx = int(tileset["firstgid"])

    for i in range(len(tile_data)):
        if tile_data[i] > 0:
            tile_data[i] -= tile_idx

    for i in range(len(col_data)):
        if col_data[i] > 0:
            col_data[i] -= col_tile_idx

    with open(args.output, "w+") as out_c_file:
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

            if col_data:
                out_c_file.write("\n")
                out_c_file.write(f"const unsigned char {output_filename}_collisions[] = " + "\n" + "{" + "\n")
                col_data_strs = ["    "]
                line_num = 0
                for i, col_idx in enumerate(col_data):
                    col_data_strs[line_num] += f"0x{col_idx:02x},"
                    if (i != 0 and (i + 1) % 16 == 0 and i != len(col_data) - 1):
                        col_data_strs[line_num] += "\n"
                        line_num += 1
                        col_data_strs.append("    ")

                col_data_strs[line_num] = col_data_strs[line_num][:-1] + "\n"
                out_c_file.writelines(col_data_strs)
                out_c_file.write("};\n")

    with open(os.path.splitext(args.output)[0] + ".h", "w+") as out_h_file:
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

            if (col_data):
                out_h_file.write(f"extern const unsigned char {output_filename}_collisions[];\n")

            out_h_file.write("\n#endif\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", required=True)
    parser.add_argument("-o", "--output", required=True)

    main(parser.parse_args())
