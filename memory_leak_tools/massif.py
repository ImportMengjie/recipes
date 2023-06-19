#!/bin/python3
import collections
import msparser
import sys


header = [
    "id",
    "time",
    "heap",
]

data = msparser.parse_file(sys.argv[1])


address_list = []

def line_to_str(line):
    s = ",".join(map(str, [line["id"], line["time"], line["heap"]] + [ line["address"][addr]['size'] if addr in line['address']  else "null" for addr in address_list]))
    return s
    


if __name__ == "__main__":
    lines = []
    for snapshot in data["snapshots"]:
        line = {
            "id": snapshot["id"],
            "time": snapshot["time"],
            "heap": snapshot["mem_heap"] / 1024,
        }
        line["address"] = collections.OrderedDict()
        for children in snapshot["heap_tree"]["children"]:
            if children["details"]:
                addr = children["details"]["address"]
                if addr not in address_list:
                    address_list.append(addr)
                    header.append('"' + children['details']["function"] + '"')
                line["address"][addr] = {
                    "size": children["nbytes"] / 1024,
                    "function": children['details']["function"],
                }
        lines.append(line)


    print(",".join(header))
    for line in lines:
        print(line_to_str(line))

