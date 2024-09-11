# !/usr/bin/env python3
# author: changkun<hi[at]changkun.de>

import os

chapters = ['00-preface.md', '01-1-introduction.md','01-2-text-input-and-output.md','01-3-common-gotchas.md', 'appendix1.md']

ignores = ['TOC', 'Table of Content', 'License', 'license']


with open('modern-cpp-tutorial.md', 'w') as outfile:
    outfile.write("""---
title: "Lập trình hệ thống"
author: Unibook <system-programming[at]unibook.com>
copyright: cc-by-nc-nd 4.0
---
""")
    for chapter in chapters:
        if os.path.isfile(chapter):
            with open(chapter) as ch:
                outfile.write('\n')
                for line in ch:
                    if any(keyword in line for keyword in ignores):
                        continue
                    else:
                        outfile.write(line)