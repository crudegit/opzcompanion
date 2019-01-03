#!/usr/bin/python3

import mido
import time
import binascii

import ctypes

class EncoderSignBitField(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ('s0', ctypes.c_uint8, 1)
        , ('s1', ctypes.c_uint8, 1)
        , ('s2', ctypes.c_uint8, 1)
        , ('s3', ctypes.c_uint8, 1)
        , ('s4', ctypes.c_uint8, 1)
        , ('s5', ctypes.c_uint8, 1)
        , ('s6', ctypes.c_uint8, 1)
        , ('s7', ctypes.c_uint8, 1)
    ]
    
    def __str__(self):
        return strstruct(self)

class OPZEncoderMapping(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ('signs1', EncoderSignBitField)
        , ('channel', ctypes.c_uint8)
        , ('white1', ctypes.c_uint8)
        , ('white2', ctypes.c_uint8)
        , ('green1', ctypes.c_uint8)
        , ('green2', ctypes.c_uint8)
        , ('green3', ctypes.c_uint8)
        , ('green4', ctypes.c_uint8)
        , ('sign2', EncoderSignBitField)
        , ('yellow1', ctypes.c_uint8)
        , ('yellow2', ctypes.c_uint8)
        , ('white3', ctypes.c_uint8)
        , ('white4', ctypes.c_uint8)
        , ('yellow3', ctypes.c_uint8)
        , ('yellow4', ctypes.c_uint8)
        , ('dummy1', ctypes.c_uint8)
        , ('sign3', EncoderSignBitField)
        , ('blue1', ctypes.c_uint8)
        , ('blue2', ctypes.c_uint8)
        , ('blue3', ctypes.c_uint8)
        , ('blue4', ctypes.c_uint8)
    ]

class OPZSysExMessage:
    def __init__(self, d, direction="I"):
        self.parts = {}
        
        if not d.startswith('F0002076'):
            raise ValueError('Not an OPZ SysEx message')
        
        d = d[8:]
        d = d[:-2]
        
        if d[:2] == '0E': # encoder mapping
            d = d[2:]
            
"""
def cdbnew(value):
    return True

mido.messages.check_databyte = cdbnew
"""

def get_port_name(port, l):
    try:
        return [x for x in filter(lambda x: x.lower().startswith(port.lower()), l)][0]
    except IndexError as e:
        raise ValueError('Could not find device %s, is it connected? List: %s' % (port, str(l)))

def strstruct(s):
    return "{}: {{{}}}".format(s.__class__.__name__,
                               ", ".join(["{}: {}".format(field[0],
                                                          getattr(s,
                                                                  field[0]))
                                          for field in s._fields_]))

def _main():
    from clint.textui import colored, puts, indent
    import sys
    import threading
    import pprint
    
    pp = pprint.PrettyPrinter(indent=2)

    wrlock = threading.RLock()
    
    f = open('midi-dump-%s.txt' % (time.strftime('%Y-%m-%d_%H%M%S',time.gmtime()), ), 'w+')

    d = {
        'opzin': None
        , 'opzout': None
        , 'ardout': None
        , 'ardin': None
    }

    parms = {
        'monitor': True
        , 'clock': False
        , 'fwd': False
        , 'sendinfo': True
        , 'dump': True
        , 'debug': False
        , 'lbr': 128
        , 'idel': 1
        , 'zinfo': '021e53 05'
        , 'readconfig': True
    }
    
    def puts_message(direction, port, m):
        
        ignore = False
        if not parms['debug']:
            v = m.hex().replace(' ','').upper()
            if v == 'F00020760100'+parms['zinfo'].replace(' ','').upper()+'F7' \
                or (v.startswith('F00020760101') and v.endswith((parms['zinfo'].replace(' ','').upper()[2:-2])+'F7')):
                ignore = True
        
        if ignore:
            return
            
        l = format_message(direction, port, m, clinty=True)
        puts(l[0])

        with indent(34):
            for i in range(1,len(l)):
                puts(l[i])
    
    def format_message(direction, port, m, clinty=False, lbr=None):
        if not clinty:
            return "< %-10s %-10s %-10s %s\n" % (direction, port, m.type, m.hex().replace(' ',''))
        
        if lbr is None:
            lbr = parms['lbr']
        
        r = []
        
        s = ''
        if direction == 'I':
            s = s + colored.green("%1s %-10s" % (direction, port))
        else:
            s = s + colored.blue("%1s %-10s" % (direction, port))
        
        s = s + colored.magenta("%-10s" % (m.type,))
        
        hm = m.hex().replace(' ','')
        if hm.startswith("F0"):
            s = s + colored.white("F0")
            hm = hm[2:]
            
            if hm.startswith('00207601'):
                s = s + colored.yellow(hm[:6])
                hm = hm[6:]
                s = s + colored.red(hm[:2])
                hm = hm[2:]
                s = s + colored.green(hm[:2])
                
                if hm.startswith('0E'):
                    sm = hm[2:-2]
                    sm = bytearray(binascii.a2b_hex(sm))
                    try:
                        emap = OPZEncoderMapping.from_buffer(sm)
                        pp.pprint(strstruct(emap))
                    except Exception as e:
                        puts(colored.red(str(e)))
                        
                hm = hm[2:]
            else:
                s = s + colored.magenta(hm[:6])
                hm = hm[6:]
            
            while len(hm) > 2:
                v = hm[:min(len(hm)-2,lbr)]
                s = s + colored.yellow(v)
                hm = hm[len(v):]
                
                if len(hm) - 2 <=lbr:
                    break
                    
                r.append(s)
                s = ''
                
            s = s + colored.white(hm[-2:])
        else:
            s =  s + colored.white("%s" % (hm,))
        
        r.append(s)
        
        return r
        
        
    def write_dump(direction, port, m):
        if parms['dump']:            
            s = format_message(direction, port, m)
            with wrlock:
                f.write(s)
                f.flush()
                
    def mm(d):
        d = ''.join(d.split(' ')).upper()
        d = binascii.a2b_hex(d)
        if d[0] == 0xf0:
            if d[-1] == 0xf7:
                d = d[:-1]
        return mido.messages.build_message(mido.messages.get_spec(d[0]), d)
    
    def sexm(d):
        d = ''.join(d.split(' ')).upper()
        if d.startswith('F0'):
            d = d[2:]
        if d.endswith('F7'):
            d = d[:-2]
        if parms['debug']: print('SysEx input: %s' % (d,))
        return mido.Message('sysex',data=binascii.a2b_hex(d))

    def mb(x):
        v = d[x]
        if v == None:
            if x == 'opzin':
                v = mido.open_input(get_port_name('OP-Z', mido.get_input_names()))
            elif x == 'opzout':
                v = mido.open_output(get_port_name('OP-Z', mido.get_output_names()))
            elif x == 'ardout':
                v = mido.open_output(get_port_name('Arduino', mido.get_output_names()))
        d[x] = v
        return v

    mido.set_backend('mido.backends.portmidi')
    
    def send_msg(o, m):
        if parms['dump']:
            write_dump('O', o, m)
            
        if parms['monitor']:
            if m.type != 'clock' or parms['clock']:
                puts_message('O', o, m)
        mb(o).send(m)
    
    def gen_incb(port):
        
        def incb(m):
            if parms['monitor']:
                if m.type != 'clock' or parms['clock']:
                    puts_message('I', port, m)
            
            write_dump('I', port, m)
            
            if parms['readconfig']:
                hm = m.hex().replace(' ','').upper()
                if hm.startswith('F00020760109'):
                    # f000207601 09 011b 00000000 0078171c6d1d
                    # 0123456789 01 2345 67890123 4567890
                    ofs = int(hm[16:24],16)
                    time.sleep(.05)
                    send_msg('opzout', mm('f000207601 0b 0009 0000 %08x 0000 f7' % (ofs,)))
            
            if parms['fwd']:
                try:
                    mb('ardout').send(m)
                except:
                    pass
        
        return incb
    
    
    
    def sendinfomessage():
        while True:
            if parms['sendinfo']:
                send_msg('opzout', sexm('00 20 76 01' + ' 00 ' + parms['zinfo']))
            time.sleep(parms['idel'])
    
    bgt = threading.Thread(target=sendinfomessage,name="SindInfoMsg")
 
    import cmd
    
    class PMCmd(cmd.Cmd):
        def __init__(self):
            cmd.Cmd.__init__(self)
            self.prompt = 'PM> '
        def do_connect(self, arg):
            print("forwarding started")
            mb('opzin').callback = gen_incb('opzin')
            send_msg('opzout', mm('f0 7e7f0601 f7'))
            time.sleep(.1)
            send_msg('opzout', mm('f000207601 00 '+parms['zinfo']+' f7'))
            time.sleep(.1)
            send_msg('opzout', mm('f000207601 0b 0009 000000000000 0000 f7'))
            time.sleep(.1   )
            bgt.start()
        def do_sexm(self, arg):
            m = sexm(arg)
            pp.pprint(m)
        def do_s(self, arg):
            self.do_send(arg)
        def do_send(self, arg):
            m = mm(arg)
            pp.pprint(m)
            send_msg('opzout', m)
        def do_exit(self, arg):
            return True
        def do_p(self, arg):
            pp.pprint(parms)
        def do_toggle(self, arg):
            parms[arg.lower()] = (not parms[arg.lower()])
            self.do_p('')
        def do_set(self, arg):
            x = arg.split(' ', 2)
            parms[x[0].lower()] = x[1]
            self.do_p('')
        def do_seti(self, arg):
            x = arg.split(' ', 2)
            parms[x[0].lower()] = int(x[1])
            self.do_p('')
        def do_mm(self, arg):
            puts_message('X', 'internal', mm(arg))
        def do_EOF(self, arg):
            return True 
        def emptyline(self):
            return False
        def do_h2a(self, arg):
            pp.pprint(binascii.a2b_hex(arg).decode('latin1'))
        def do_bdec(self, arg):
            from bitstring import BitArray
            
            if arg == None:
                arg = '00166B56174A6B3733755240363E145E2A6D41074D213C3A157217264B6C63735E1E1E5F6A7A3B320E743B747A3B694C6D453C6F2B5E4E6D3D7D6C7B3175480A7C153362631A46325E51370977275F3A12733D4B3E5F0E7B735F5F492E6557497E6E357C7F3E1D4E7A44557D1F2F53032B7F4E7C534A7458456E6E43172E4F37556E7D75665F3F5D6C733E6D493F175C2C6F0A675D18326F306F77657B045A7E7D454A4038321C7530547D0A5661633A2E1B371F5767217B17041679577A7E56517B077A79266B766E7B002A72'
            
            c = BitArray(hex=arg)
            
            ocl = len(c.bin)
            for x in range(int(ocl/8)-2):
                del c[x*7]
            
            s = c[:int(len(c.bin)/8)*8].bytes
            pp.pprint(s.decode('latin1'))
            pp.pprint(binascii.b2a_hex(s))
            
        def onecmd(self, c):
            try:
                return cmd.Cmd.onecmd(self, c)
            except Exception as e:
                import traceback
                
                puts(colored.red("Error executing command: %s" % (c,)))
                with indent(4):
                    puts(colored.yellow(repr(e)))
                    puts(colored.yellow(traceback.format_exc()))
            return False
        def do_default(self, arg):
            puts(colored.magenta("Unknown command %s\n" % (arg,)))
    
    c = PMCmd()
 
    c.cmdloop()
    if bgt.is_alive():
        bgt.join(timeout=1)

    sys.exit()    

if __name__ == '__main__':
    _main()

