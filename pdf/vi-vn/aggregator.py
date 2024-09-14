# !/usr/bin/env python3
# author: changkun<hi[at]changkun.de>

import os

chapters = [
    '00-preface.md', 
    '01-1-introduction.md',
    '01-2-text-input-and-output.md',
    '01-3-common-gotchas.md', 
    '01-4-string-and-struct.md',
    '01-5-debugging.md',
    '01-6-review-question.md',
    '02-1-kernel-shells-terminals.md',
    '02-2-process-introduction.md',
    '02-3-porking-introduction.md',
    '02-4-fork-exec-wait.md',
    '02-5-wait-macros-using-signals.md',
    '02-6-review-question.md',
    '03-1-memory-introduction.md',
    '03-2-memory-allocator.md',
    '03-3-smashing-example.md',
    '03-4-review-questions.md',
    '04-1-pthread-introduction.md',
    '04-3-parallel-problems.md',
    '04-4-reviews-questions.md',
    'appendix1.md']

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