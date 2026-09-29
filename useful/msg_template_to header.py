#!/usr/bin/env python3

# Convert Second Life message_template.msg into a C header
# containing MSG_* IDs with message fields shown as comments.
#
# Usage:
#     ./msg_template_to_header.py message_template.msg > message_template.h

import re
import sys

MACROS = {
    "High": "MK_HIGH",
    "Medium": "MK_MED",
    "Low": "MK_LOW",
    "Fixed": "MK_FIXED",
}

message_re = re.compile(
    r"^\s*(\w+)\s+(High|Medium|Low|Fixed)\s+"
    r"(0x[0-9A-Fa-f]+|\d+)"
)

field_re = re.compile(
    r"^\s*\{\s*(\w+)\s+(\w+)\s*\}"
)

with open(sys.argv[1], encoding="utf-8") as f:
    lines = f.readlines()

messages = []

i = 0

while i < len(lines):
    m = message_re.match(lines[i])

    if not m:
        i += 1
        continue

    name, frequency, number = m.groups()
    fields = []

    # Find the message's opening { immediately before its fields.
    i += 1

    depth = 0

    while i < len(lines):
        line = lines[i]

        fm = field_re.match(line)
        if fm:
            field_name, field_type = fm.groups()
            fields.append(f"{field_name}: {field_type}")

        depth += line.count("{")
        depth -= line.count("}")

        # Message block has closed.
        if depth < 0:
            break

        i += 1

    messages.append((name, frequency, number, fields))

print("""#define MK_HIGH(n)  (0x00000u | (n))
#define MK_MED(n)   (0x10000u | (n))
#define MK_LOW(n)   (0x20000u | (n))
#define MK_FIXED(n) (0x30000u | (n))

enum {
""")

for name, frequency, number, fields in messages:
    macro = MACROS[frequency]

    comment = ""
    if fields:
        comment = " // " + ", ".join(fields)

    print(f"    MSG_{name:<32} = {macro}({number}),{comment}")

print("};")