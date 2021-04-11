from intelhex import IntelHex
import os


def calcCopyAdr(adr):
    if cfgDic['prichar'] <= adr < (cfgDic['prichar'] + cfgDic['pricharnum']):
        return (adr - cfgDic['prichar']) * cfgDic['fontbytesize'] + cfgDic['fontpriadr']
    elif cfgDic['secchar'] <= adr < (cfgDic['secchar'] + cfgDic['seccharnum']):
        return (adr - cfgDic['secchar']) * cfgDic['fontbytesize'] + cfgDic['fontsecadr']
    else:
        return -1


def charCopy(inthex_obj, adr_source, adr_dest):
    adr = calcCopyAdr(adr_source)
    if adr == -1:
        print('Error source addres:' + hex(adr_source))
        return -1
    else:
        adr_source = adr
    adr = calcCopyAdr(adr_dest)
    if adr == -1:
        print('Error destination addres:' + hex(adr_dest))
        return -1
    else:
        adr_dest = adr
    for _ in range(cfgDic['fontbytesize']):
        tmp = inthex_obj[adr_source]
        inthex_obj[adr_dest] = tmp
        adr_source += 1
        adr_dest += 1
    return 0


def import_font(inthex_obj, file):
    char_s = 0
    get_char = 0
    adr = cfgDic['fontpriadr']
    for S in file:
        S = S.strip()
        if len(S) < 2 or (S[0] != 'd' and S[0] != 'D') or (S[1] != 'b' and S[1] != 'B'): continue
        dig = ''
        for c in S[2:]:
            if c == ';':
                break
            elif c == ' ' or c == '\t' or c == ',':
                continue
            elif '0' <= c <= '9' or 'a' <= c <= 'f' or 'A' <= c <= 'F':
                dig += c
            elif c == 'h' or c == 'H':
                dig = '0x' + dig
                if char_s == 0:
                    inthex_obj[adr] = 0
                    adr += 1
                    inthex_obj[adr] = 0
                    adr += 1
                    char_s = 2
                inthex_obj[adr] = int(dig, 16)
                dig = ''
                adr += 1
                char_s += 1
                if char_s == cfgDic['fontbytesize']:
                    get_char += 1
                    char_s = 0
                    if adr == cfgDic['fontpriadr'] + cfgDic['fontbytesize'] * cfgDic['pricharnum']:
                        print('Import char from ' + hex(cfgDic['prichar']) + ' to ' + hex(
                            cfgDic['prichar'] + cfgDic['pricharnum'] - 1) + ' done.')
                        print('Destination from ' + hex(cfgDic['fontpriadr']) + ' to ' + hex(adr - 1))
                        if cfgDic['fontsecadr'] != 0 and cfgDic['secchar'] > (
                                cfgDic['prichar'] + cfgDic['pricharnum'] - 1):
                            adr = cfgDic['fontsecadr']
                        else:
                            return 1
                    elif adr == cfgDic['fontsecadr'] + cfgDic['fontbytesize'] * cfgDic['seccharnum']:
                        print('Import char from ' + hex(cfgDic['secchar']) + ' to ' + hex(
                            cfgDic['secchar'] + cfgDic['seccharnum'] - 1) + ' done.')
                        print('Destination from ' + hex(cfgDic['fontsecadr']) + ' to ' + hex(adr - 1))
                        return 1
            else:
                break
    else:
        if get_char == cfgDic['pricharnum'] + cfgDic['seccharnum']:
            return 1
        else:
            print('Error, load' + int(get_char) + 'char, must be ' + int(cfgDic['pricharnum'] + cfgDic['seccharnum']))
            return 0


def CalcWidthByte(input_byte):
    for i in range(8):
        if input_byte & 1:
            return 8 - i
        else:
            input_byte >>= 1
    return 0


def SetWidth(inthex_obj, char_num):
    e = 0
    a = calcCopyAdr(char_num)
    if a == -1: return -1
    a += 2
    if cfgDic['fontbytesize'] == 74:
        for _ in range(2, cfgDic['fontbytesize'], 3):
            b = inthex_obj[a]
            a += 1
            c = inthex_obj[a]
            a += 1
            d = inthex_obj[a]
            a += 1
            if d == 0:
                if c == 0:
                    if b == 0:
                        continue
                    else:
                        f = CalcWidthByte(b)
                        if f > e: e = f
                else:
                    f = CalcWidthByte(c) + 8
                    if f > e: e = f
            else:
                f = CalcWidthByte(d) + 16
                if f > e: e = f
    elif cfgDic['fontbytesize'] == 34:
        for _ in range(2, cfgDic['fontbytesize'], 2):
            b = inthex_obj[a]
            a += 1
            c = inthex_obj[a]
            a += 1
            if c == 0:
                if b == 0:
                    continue
                else:
                    f = CalcWidthByte(b)
                    if f > e: e = f
            else:
                f = CalcWidthByte(c) + 8
                if f > e: e = f
    else:
        for _ in range(2, cfgDic['fontbytesize'], 4):
            b = inthex_obj[a]
            a += 1
            c = inthex_obj[a]
            a += 1
            d = inthex_obj[a]
            a += 1
            w = inthex_obj[a]
            a += 1
            if w == 0:
                if d == 0:
                    if c == 0:
                        if b == 0:
                            continue
                        else:
                            f = CalcWidthByte(b)
                            if f > e: e = f
                    else:
                        f = CalcWidthByte(c) + 8
                        if f > e: e = f
                else:
                    f = CalcWidthByte(d) + 16
                    if f > e: e = f
            else:
                f = CalcWidthByte(w) + 24
                if f > e: e = f
    if e == 0: e = cfgDic['fontspacesize']
    inthex_obj[calcCopyAdr(char_num) + 1] = e
    return 1


def writeCfgDirTbl(src_str, dest_str):
    if 'CopyCharTbl' in cfgDic:
        cfgDic['CopyCharTbl'].append([int(src_str, 16), int(dest_str, 16)])
    else:
        cfgDic['CopyCharTbl'] = [[int(src_str, 16), int(dest_str, 16)], ]


def getCopyAdr(str):
    src_str = ''
    dest_str = ''
    dest = 0
    f_str = cF[str].lower()
    indx = f_str.find('copycharfromto=')
    if indx == -1:
        indx = 0
    else:
        indx = indx + len('copycharfromto=')
    for c in cF[str][indx:]:
        if '0' <= c <= '9' or 'a' <= c <= 'f' or 'A' <= c <= 'F' or c == 'x' or c == 'X':
            if dest == 0:
                src_str += c
            else:
                dest_str += c
        elif c == ':':
            if src_str != '':
                dest = 1
            else:
                print('Error copy char table in line' + str(i))
                return 0
        elif c == ',':
            if src_str != '' and dest_str != '':
                writeCfgDirTbl(src_str, dest_str)
            else:
                print('Error copy char table in line' + str(i))
                return 0
            dest = 0
            src_str = ''
            dest_str = ''
        elif c == '\t' or c == ' ':
            continue
        elif c == '#':
            if src_str != '' and dest_str != '':
                writeCfgDirTbl(src_str, dest_str)
                return 0
            else:
                return 1
    else:
        if src_str != '' and dest_str != '':
            writeCfgDirTbl(src_str, dest_str)
            return 0
        else:
            return 1


def readFontCfg(cF, num_str):
    delDic = 0
    copyChar = 0
    cfgDic['asmfile'] = ''
    print('----------------Reading Font2Hex.cfg----------------')
    for i in range(num_str, len(cF)):
        tmp = cF[i].find('#')
        if tmp != -1: cF[i] = cF[i][:tmp]
        tmp = cF[i].find('\n')
        if tmp != -1: cF[i] = cF[i][:tmp]
        if cF[i] == '':
            continue
        f_str = cF[i].lower()
        if copyChar == 1:
            if getCopyAdr(i) == 1:
                continue
            else:
                if 'CopyCharTbl' in cfgDic:
                    print('Import from Font2Hex.cfg Copy Char Table:')
                    for j in range(len(cfgDic['CopyCharTbl'])):
                        print(hex(cfgDic['CopyCharTbl'][j][0]) + '=>' + hex(cfgDic['CopyCharTbl'][j][1]))
                copyChar = 0
        for cmd in cfgDic:
            tmp = f_str.find(cmd + '=')
            if tmp != -1:
                if type(cfgDic[cmd]) == str:
                    if cmd == 'asmfile':  # AsmFile
                        if cfgDic[cmd] == '':
                            print('Asm file: ' + cF[i][tmp + len(cmd) + 1:])
                        else:
                            return i
                    cfgDic[cmd] = cF[i][tmp + len(cmd) + 1:]
                    cfgDic[cmd] = cfgDic[cmd].strip()
                    if cmd == 'hexoutfile':  # HexOutFile
                        print('Out hex file: ' + cfgDic[cmd])
                    elif cmd == 'txtoutfile':  # TxtOutFile
                        print('Out txt file: ' + cfgDic[cmd])
                    elif cmd == 'fontrange':  # FontRange
                        print('Range: ' + cfgDic[cmd])
                        tmp = cfgDic[cmd].find('-')
                        if tmp != -1:
                            cfgDic['prichar'] = int(cfgDic[cmd][0:tmp], 16)
                            print('Primary Char from: ' + hex(cfgDic['prichar']))
                            tmp1 = cfgDic[cmd].find(',', tmp + 1)
                            if tmp1 != -1:
                                cfgDic['pricharnum'] = int(cfgDic[cmd][tmp + 1:tmp1], 16) - cfgDic['prichar'] + 1
                                print('Primary Char quantity: ' + str(cfgDic['pricharnum']))
                                tmp = cfgDic[cmd].find('-', tmp + 1)
                                if tmp != -1:
                                    cfgDic['secchar'] = int(cfgDic[cmd][tmp1 + 1:tmp], 16)
                                    print('Secondary Char from: ' + hex(cfgDic['secchar']))
                                    cfgDic['seccharnum'] = int(cfgDic[cmd][tmp + 1:], 16) - cfgDic['secchar'] + 1
                                    print('Secondary Char quantity: ' + str(cfgDic['seccharnum']))
                                else:
                                    print('Error, range in cfg file not correct! line=' + str(i))
                                    return -1
                            else:
                                cfgDic['pricharnum'] = int(cfgDic[cmd][tmp + 1:], 16) - cfgDic['prichar'] + 1
                                print('Primary Char quantity: ' + str(cfgDic['pricharnum']))
                        else:
                            print('Range in cfg file not correct! line=' + str(i))
                            return -1
                    elif cmd == 'copycharfromto':  # CopyCharFromTo
                        cfgDic[cmd] = cfgDic[cmd].lower()
                        if 'CopyCharTbl' in cfgDic:
                            delDic = 1
                        if cfgDic[cmd] == 'no':
                            print('Not Copy Char Table.')
                        else:
                            if getCopyAdr(i) == 1:
                                copyChar = 1
                    break
                elif type(cfgDic[cmd]) == int:
                    if cmd == 'fontpriadr' or cmd == 'fontsecadr':
                        cfgDic[cmd] = int(cF[i][tmp + len(cmd) + 1:], 16)
                        if cmd == 'fontpriadr':
                            print('Begin of destination Primary char range: ' + hex(cfgDic[cmd]))
                        else:
                            print('Begin of destination Secondary char range: ' + hex(cfgDic[cmd]))
                    else:
                        cfgDic[cmd] = int(cF[i][tmp + len(cmd) + 1:])
                        if cmd == 'fontbytesize':
                            print('Font size in bytes: ' + str(cfgDic[cmd]))
                        elif cmd == 'fontspacesize':
                            print('Space size in pixels: ' + str(cfgDic[cmd]))
                        else:
                            print(cmd + '=>' + str(cfgDic[cmd]))
                    break
        if delDic == 1:
            del cfgDic['CopyCharTbl']
            print('Delete copy char table.')
            delDic = 0
    else:
        if cfgDic['asmfile'] == '':
            print('Error, not define asm file!')
            return -1
        else:
            return 0


cfgDic = dict(asmfile='',
              hexoutfile='',
              txtoutfile='',
              fontpriadr=0,
              fontsecadr=0,
              fontrange='',
              fontbytesize=0,
              fontspacesize=0,
              copycharfromto='',
              prichar=0,
              secchar=0,
              pricharnum=0,
              seccharnum=0)

if os.path.exists('font2hex.cfg'):
    cfgFile = open('font2hex.cfg', 'r')
    cF = cfgFile.readlines()
    cfgFile.close()
    cfgLine = 0
    ih = IntelHex()
    while 1:
        finish_flag = 0
        cfgLine = readFontCfg(cF, cfgLine)
        if cfgLine != -1:
            if cfgLine == 0:
                print('----------------Cfg File read fully----------------')
            else:
                print('----------------Cfg File read to ' + str(cfgLine) + ' line----------------')
        try:
            inFile = open(cfgDic['asmfile'], 'r')
        except FileNotFoundError:
            print('File ' + cfgDic['asmfile'] + ' not found.')
            break
        print('~~~~~~~Import begin from file  ' + cfgDic['asmfile'] + '~~~~~~~')
        finish_flag = import_font(ih, inFile)
        inFile.close()
        if finish_flag == 1:
            for i in range(cfgDic['prichar'], cfgDic['prichar'] + cfgDic['pricharnum']):
                if SetWidth(ih, i) == -1:
                    print('Error, char not in range=>' + hex(i))
                    finish_flag = 0
                    break
            if finish_flag == 0: break
            if cfgDic['secchar'] != 0 and cfgDic['seccharnum'] != 0:
                for i in range(cfgDic['secchar'], cfgDic['secchar'] + cfgDic['seccharnum']):
                    if SetWidth(ih, i) == -1:
                        print('Error, char not in range=>' + hex(i))
                        finish_flag = 0
                        break
            else:
                print('Second char range not present.')
            if finish_flag == 0: break
            if 'CopyCharTbl' in cfgDic:
                print('---Copy char From Table---')
                for i in range(len(cfgDic['CopyCharTbl'])):
                    print(hex(cfgDic['CopyCharTbl'][i][0]) + '=>' + hex(cfgDic['CopyCharTbl'][i][1]))
                    if charCopy(ih, cfgDic['CopyCharTbl'][i][0], cfgDic['CopyCharTbl'][i][1]) == -1:
                        break
        else:
            print('Error size font datain file: ' + cfgDic['asmfile'])
            break
        if cfgLine == 0: break
    if finish_flag == 1:
        if cfgDic['hexoutfile'] != '':
            ih.write_hex_file(cfgDic['hexoutfile'], False)
            print('Hex File wrote.')
        if cfgDic['txtoutfile'] != '':
            outFile = open(cfgDic['txtoutfile'], 'w')
            ih.dump(outFile)
            outFile.close()
            print('Txt File wrote.')
else:
    print("Error can't open file font2hex.cfg!")
