#!/usr/bin/python333
# -*- coding: utf-8 -*-

# 转换json文件中的"\uxxxx"显示出真实字符

import json, sys

data = json.load(sys.stdin)
json.dump(data, sys.stdout, ensure_ascii=False, indent=4)
