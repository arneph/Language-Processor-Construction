#include "JavaCode.h"

JavaInstruction nop = {0x00, "nop", 0};
JavaInstruction aconst_null = {0x01, "aconst_null", 0};
JavaInstruction iconst_m1 = {0x02, "iconst_m1", 0};
JavaInstruction iconst_0 = {0x03, "iconst_0", 0};
JavaInstruction iconst_1 = {0x04, "iconst_1", 0};
JavaInstruction iconst_2 = {0x05, "iconst_2", 0};
JavaInstruction iconst_3 = {0x06, "iconst_3", 0};
JavaInstruction iconst_4 = {0x07, "iconst_4", 0};
JavaInstruction iconst_5 = {0x08, "iconst_5", 0};
JavaInstruction lconst_0 = {0x09, "lconst_0", 0};
JavaInstruction lconst_1 = {0x0a, "lconst_1", 0};
JavaInstruction fconst_0 = {0x0b, "fconst_0", 0};
JavaInstruction fconst_1 = {0x0c, "fconst_1", 0};
JavaInstruction fconst_2 = {0x0d, "fconst_2", 0};
JavaInstruction dconst_0 = {0x0e, "dconst_0", 0};
JavaInstruction dconst_1 = {0x0f, "dconst_1", 0};
JavaInstruction bipush = {0x10, "bipush", 1};
JavaInstruction sipush = {0x11, "sipush", 2};
JavaInstruction ldc = {0x12, "ldc", 1};
JavaInstruction ldc_w = {0x13, "ldc_w", 2};
JavaInstruction ldc2_w = {0x14, "ldc2_w", 2};
JavaInstruction iload = {0x15, "iload", 1};
JavaInstruction lload = {0x16, "lload", 1};
JavaInstruction fload = {0x17, "fload", 1};
JavaInstruction dload = {0x18, "dload", 1};
JavaInstruction aload = {0x19, "aload", 1};
JavaInstruction iload_0 = {0x1a, "iload_0", 0};
JavaInstruction iload_1 = {0x1b, "iload_1", 0};
JavaInstruction iload_2 = {0x1c, "iload_2", 0};
JavaInstruction iload_3 = {0x1d, "iload_3", 0};
JavaInstruction lload_0 = {0x1e, "lload_0", 0};
JavaInstruction lload_1 = {0x1f, "lload_1", 0};
JavaInstruction lload_2 = {0x20, "lload_2", 0};
JavaInstruction lload_3 = {0x21, "lload_3", 0};
JavaInstruction fload_0 = {0x22, "fload_0", 0};
JavaInstruction fload_1 = {0x23, "fload_1", 0};
JavaInstruction fload_2 = {0x24, "fload_2", 0};
JavaInstruction fload_3 = {0x25, "fload_3", 0};
JavaInstruction dload_0 = {0x26, "dload_0", 0};
JavaInstruction dload_1 = {0x27, "dload_1", 0};
JavaInstruction dload_2 = {0x28, "dload_2", 0};
JavaInstruction dload_3 = {0x29, "dload_3", 0};
JavaInstruction aload_0 = {0x2a, "aload_0", 0};
JavaInstruction aload_1 = {0x2b, "aload_1", 0};
JavaInstruction aload_2 = {0x2c, "aload_2", 0};
JavaInstruction aload_3 = {0x2d, "aload_3", 0};
JavaInstruction iaload = {0x2e, "iaload", 0};
JavaInstruction laload = {0x2f, "laload", 0};
JavaInstruction faload = {0x30, "faload", 0};
JavaInstruction daload = {0x31, "daload", 0};
JavaInstruction aaload = {0x32, "aaload", 0};
JavaInstruction baload = {0x33, "baload", 0};
JavaInstruction caload = {0x34, "caload", 0};
JavaInstruction saload = {0x35, "saload", 0};
JavaInstruction istore = {0x36, "istore", 1};
JavaInstruction lstore = {0x37, "lstore", 1};
JavaInstruction fstore = {0x38, "fstore", 1};
JavaInstruction dstore = {0x39, "dstore", 1};
JavaInstruction astore = {0x3a, "astore", 1};
JavaInstruction istore_0 = {0x3b, "istore_0", 0};
JavaInstruction istore_1 = {0x3c, "istore_1", 0};
JavaInstruction istore_2 = {0x3d, "istore_2", 0};
JavaInstruction istore_3 = {0x3e, "istore_3", 0};
JavaInstruction lstore_0 = {0x3f, "lstore_0", 0};
JavaInstruction lstore_1 = {0x40, "lstore_1", 0};
JavaInstruction lstore_2 = {0x41, "lstore_2", 0};
JavaInstruction lstore_3 = {0x42, "lstore_3", 0};
JavaInstruction fstore_0 = {0x43, "fstore_0", 0};
JavaInstruction fstore_1 = {0x44, "fstore_1", 0};
JavaInstruction fstore_2 = {0x45, "fstore_2", 0};
JavaInstruction fstore_3 = {0x46, "fstore_3", 0};
JavaInstruction dstore_0 = {0x47, "dstore_0", 0};
JavaInstruction dstore_1 = {0x48, "dstore_1", 0};
JavaInstruction dstore_2 = {0x49, "dstore_2", 0};
JavaInstruction dstore_3 = {0x4a, "dstore_3", 0};
JavaInstruction astore_0 = {0x4b, "astore_0", 0};
JavaInstruction astore_1 = {0x4c, "astore_1", 0};
JavaInstruction astore_2 = {0x4d, "astore_2", 0};
JavaInstruction astore_3 = {0x4e, "astore_3", 0};
JavaInstruction iastore = {0x4f, "iastore", 0};
JavaInstruction lastore = {0x50, "lastore", 0};
JavaInstruction fastore = {0x51, "fastore", 0};
JavaInstruction dastore = {0x52, "dastore", 0};
JavaInstruction aastore = {0x53, "aastore", 0};
JavaInstruction bastore = {0x54, "bastore", 0};
JavaInstruction castore = {0x55, "castore", 0};
JavaInstruction sastore = {0x56, "sastore", 0};
JavaInstruction pop = {0x57, "pop", 0};
JavaInstruction pop2 = {0x58, "pop2", 0};
JavaInstruction dup = {0x59, "dup", 0};
JavaInstruction dup_x1 = {0x5a, "dup_x1", 0};
JavaInstruction dup_x2 = {0x5b, "dup_x2", 0};
JavaInstruction dup2 = {0x5c, "dup2", 0};
JavaInstruction dup2_x1 = {0x5d, "dup2_x1", 0};
JavaInstruction dup2_x2 = {0x5e, "dup2_x2", 0};
JavaInstruction swap_ = {0x5f, "swap", 0};
JavaInstruction iadd = {0x60, "iadd", 0};
JavaInstruction ladd = {0x61, "ladd", 0};
JavaInstruction fadd = {0x62, "fadd", 0};
JavaInstruction dadd = {0x63, "dadd", 0};
JavaInstruction isub = {0x64, "isub", 0};
JavaInstruction lsub = {0x65, "lsub", 0};
JavaInstruction fsub = {0x66, "fsub", 0};
JavaInstruction dsub = {0x67, "dsub", 0};
JavaInstruction imul = {0x68, "imul", 0};
JavaInstruction lmul = {0x69, "lmul", 0};
JavaInstruction fmul = {0x6a, "fmul", 0};
JavaInstruction dmul = {0x6b, "dmul", 0};
JavaInstruction idiv = {0x6c, "idiv", 0};
JavaInstruction ldiv_ = {0x6d, "ldiv", 0};
JavaInstruction fdiv = {0x6e, "fdiv", 0};
JavaInstruction ddiv = {0x6f, "ddiv", 0};
JavaInstruction irem = {0x70, "irem", 0};
JavaInstruction lrem = {0x71, "lrem", 0};
JavaInstruction frem = {0x72, "frem", 0};
JavaInstruction drem = {0x73, "drem", 0};
JavaInstruction ineg = {0x74, "ineg", 0};
JavaInstruction lneg = {0x75, "lneg", 0};
JavaInstruction fneg = {0x76, "fneg", 0};
JavaInstruction dneg = {0x77, "dneg", 0};
JavaInstruction ishl = {0x78, "ishl", 0};
JavaInstruction lshl = {0x79, "lshl", 0};
JavaInstruction ishr = {0x7a, "ishr", 0};
JavaInstruction lshr = {0x7b, "lshr", 0};
JavaInstruction iushr = {0x7c, "iushr", 0};
JavaInstruction lushr = {0x7d, "lushr", 0};
JavaInstruction iand = {0x7e, "iand", 0};
JavaInstruction land = {0x7f, "land", 0};
JavaInstruction ior = {0x80, "ior", 0};
JavaInstruction lor = {0x81, "lor", 0};
JavaInstruction ixor = {0x82, "ixor", 0};
JavaInstruction lxor = {0x83, "lxor", 0};
JavaInstruction iinc = {0x84, "iinc", 2};
JavaInstruction i2l = {0x85, "i2l", 0};
JavaInstruction i2f = {0x86, "i2f", 0};
JavaInstruction i2d = {0x87, "i2d", 0};
JavaInstruction l2i = {0x88, "l2i", 0};
JavaInstruction l2f = {0x89, "l2f", 0};
JavaInstruction l2d = {0x8a, "l2d", 0};
JavaInstruction f2i = {0x8b, "f2i", 0};
JavaInstruction f2l = {0x8c, "f2l", 0};
JavaInstruction f2d = {0x8d, "f2d", 0};
JavaInstruction d2i = {0x8e, "d2i", 0};
JavaInstruction d2l = {0x8f, "d2l", 0};
JavaInstruction d2f = {0x90, "d2f", 0};
JavaInstruction i2b = {0x91, "i2b", 0};
JavaInstruction i2c = {0x92, "i2c", 0};
JavaInstruction i2s = {0x93, "i2s", 0};
JavaInstruction lcmp = {0x94, "lcmp", 0};
JavaInstruction fcmpl = {0x95, "fcmpl", 0};
JavaInstruction fcmpg = {0x96, "fcmpg", 0};
JavaInstruction dcmpl = {0x97, "dcmpl", 0};
JavaInstruction dcmpg = {0x98, "dcmpg", 0};
JavaInstruction ifeq = {0x99, "ifeq", 2};
JavaInstruction ifne = {0x9a, "ifne", 2};
JavaInstruction iflt = {0x9b, "iflt", 2};
JavaInstruction ifge = {0x9c, "ifge", 2};
JavaInstruction ifgt = {0x9d, "ifgt", 2};
JavaInstruction ifle = {0x9e, "ifle", 2};
JavaInstruction if_icmpeq = {0x9f, "if_icmpeq", 2};
JavaInstruction if_icmpne = {0xa0, "if_icmpne", 2};
JavaInstruction if_icmplt = {0xa1, "if_icmplt", 2};
JavaInstruction if_icmpge = {0xa2, "if_icmpge", 2};
JavaInstruction if_icmpgt = {0xa3, "if_icmpgt", 2};
JavaInstruction if_icmple = {0xa4, "if_icmple", 2};
JavaInstruction if_acmpeq = {0xa5, "if_acmpeq", 2};
JavaInstruction if_acmpne = {0xa6, "if_acmpne", 2};
JavaInstruction goto_ = {0xa7, "goto", 2};
JavaInstruction jsr = {0xa8, "jsr", 2};
JavaInstruction ret = {0xa9, "ret", 1};
JavaInstruction tableswitch = {0xaa, "tableswitch", 1};
JavaInstruction lookupswitch = {0xab, "lookupswitch", 8};
JavaInstruction ireturn = {0xac, "ireturn", 0};
JavaInstruction lreturn = {0xad, "lreturn", 0};
JavaInstruction freturn = {0xae, "freturn", 0};
JavaInstruction dreturn = {0xaf, "dreturn", 0};
JavaInstruction areturn = {0xb0, "areturn", 0};
JavaInstruction return_ = {0xb1, "return", 0};
JavaInstruction getstatic = {0xb2, "getstatic", 2};
JavaInstruction putstatic = {0xb3, "putstatic", 2};
JavaInstruction getfield = {0xb4, "getfield", 2};
JavaInstruction putfield = {0xb5, "putfield", 2};
JavaInstruction invokevirtual = {0xb6, "invokevirtual", 2};
JavaInstruction invokespecial = {0xb7, "invokespecial", 2};
JavaInstruction invokestatic = {0xb8, "invokestatic", 2};
JavaInstruction invokeinterface = {0xb9, "invokeinterface", 4};
JavaInstruction invokedynamic = {0xba, "invokedynamic", 4};
JavaInstruction new_ = {0xbb, "new", 2};
JavaInstruction newarray = {0xbc, "newarray", 1};
JavaInstruction anewarray = {0xbd, "anewarray", 2};
JavaInstruction arraylength = {0xbe, "arraylength", 0};
JavaInstruction athrow = {0xbf, "athrow", 0};
JavaInstruction checkcast = {0xc0, "checkcast", 2};
JavaInstruction instanceof = {0xc1, "instanceof", 2};
JavaInstruction monitorenter = {0xc2, "monitorenter", 0};
JavaInstruction monitorexit = {0xc3, "monitorexit", 0};
JavaInstruction wide = {0xc4, "wide", 3};
JavaInstruction multianewarray = {0xc5, "multianewarray", 3};
JavaInstruction ifnull = {0xc6, "ifnull", 2};
JavaInstruction ifnonnull = {0xc7, "ifnonnull", 2};
JavaInstruction goto_w = {0xc8, "goto_w", 4};
JavaInstruction jsr_w = {0xc9, "jsr_w", 4};
JavaInstruction breakpoint = {0xca, "breakpoint", 0};

JavaInstruction impdep1 = {0xfe, "impdep1", 0};
JavaInstruction impdep2 = {0xff, "impdep2", 0};

JavaInstruction InstrTable[] = {
    nop,
    aconst_null,
    iconst_m1,
    iconst_0,
    iconst_1,
    iconst_2,
    iconst_3,
    iconst_4,
    iconst_5,
    lconst_0,
    lconst_1,
    fconst_0,
    fconst_1,
    fconst_2,
    dconst_0,
    dconst_1,
    bipush,
    sipush,
    ldc,
    ldc_w,
    ldc2_w,
    iload,
    lload,
    fload,
    dload,
    aload,
    iload_0,
    iload_1,
    iload_2,
    iload_3,
    lload_0,
    lload_1,
    lload_2,
    lload_3,
    fload_0,
    fload_1,
    fload_2,
    fload_3,
    dload_0,
    dload_1,
    dload_2,
    dload_3,
    aload_0,
    aload_1,
    aload_2,
    aload_3,
    iaload,
    laload,
    faload,
    daload,
    aaload,
    baload,
    caload,
    saload,
    istore,
    lstore,
    fstore,
    dstore,
    astore,
    istore_0,
    istore_1,
    istore_2,
    istore_3,
    lstore_0,
    lstore_1,
    lstore_2,
    lstore_3,
    fstore_0,
    fstore_1,
    fstore_2,
    fstore_3,
    dstore_0,
    dstore_1,
    dstore_2,
    dstore_3,
    astore_0,
    astore_1,
    astore_2,
    astore_3,
    iastore,
    lastore,
    fastore,
    dastore,
    aastore,
    bastore,
    castore,
    sastore,
    pop,
    pop2,
    dup,
    dup_x1,
    dup_x2,
    dup2,
    dup2_x1,
    dup2_x2,
    swap_,
    iadd,
    ladd,
    fadd,
    dadd,
    isub,
    lsub,
    fsub,
    dsub,
    imul,
    lmul,
    fmul,
    dmul,
    idiv,
    ldiv_,
    fdiv,
    ddiv,
    irem,
    lrem,
    frem,
    drem,
    ineg,
    lneg,
    fneg,
    dneg,
    ishl,
    lshl,
    ishr,
    lshr,
    iushr,
    lushr,
    iand,
    land,
    ior,
    lor,
    ixor,
    lxor,
    iinc,
    i2l,
    i2f,
    i2d,
    l2i,
    l2f,
    l2d,
    f2i,
    f2l,
    f2d,
    d2i,
    d2l,
    d2f,
    i2b,
    i2c,
    i2s,
    lcmp,
    fcmpl,
    fcmpg,
    dcmpl,
    dcmpg,
    ifeq,
    ifne,
    iflt,
    ifge,
    ifgt,
    ifle,
    if_icmpeq,
    if_icmpne,
    if_icmplt,
    if_icmpge,
    if_icmpgt,
    if_icmple,
    if_acmpeq,
    if_acmpne,
    goto_,
    jsr,
    ret,
    tableswitch,
    lookupswitch,
    ireturn,
    lreturn,
    freturn,
    dreturn,
    areturn,
    return_,
    getstatic,
    putstatic,
    getfield,
    putfield,
    invokevirtual,
    invokespecial,
    invokestatic,
    invokeinterface,
    invokedynamic,
    new_,
    newarray,
    anewarray,
    arraylength,
    athrow,
    checkcast,
    instanceof,
    monitorenter,
    monitorexit,
    wide,
    multianewarray,
    ifnull,
    ifnonnull,
    goto_w,
    jsr_w,
    breakpoint,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    impdep1,
    impdep2
};
