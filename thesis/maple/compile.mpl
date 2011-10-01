# $Source: /u/maple/research/share.new/compile/RCS/compile.mpl,v $
#--> compile(f,options); compile a Maple procedure for exection
#
# The compile command takes as input a Maple procedure F, translates
# the code into a C function, compiles the C function.  It returns
# as it's result a Maple procedure G which when called executes the
# compiled code and returns the results computed to the Maple session.
#
# The optional arguments are
#
# optimized -- run Maple optimizer during translation
# ansi -- generate ANSI C code (default Kernighan and Ritchie C)
# language = C | fortran (not avialable) -- target language (default C)
# compiler = <symbol> -- name of the compiler (default cc)
# precision = single | double -- single or double precision
# directory = <symbol> -- name of the directory for code (default .)
# declarations = symbol::type | list(symbol::type) -- type declarations 
#
# For Maple V Release 4, the compile command is a prototype.
# It communicates data from Maple to the C executable via files.
# It creates a main program which will read data from the input
# file, call the C function, and write the results to an output file.
# Thus upon executing the procedure G, the following occors
#
# 1: The type of the Maple arguments is checked.
# 2: The arguments are written to the input file.
# 3: The C executable is executed: the data is read from the
#    input file, the C function is executed and the results are
#    written to the output file.
# 4: The Maple procedure G reads the results from the output file
# 5: The type of the output results is checked.
# 6: The results are returned to the user session.
# 
# The data types handled by the compiler at present are integers, floats,
# and one and two dimensional arrays of integers and floats.
# Subprocedures are not handled.  Global variables are not handled.
# 
# Author MBM: 96/97
#

macro( MAKEINTREP = readlib(`intrep/makeintrep`) );
macro( GET = `compile/getintrep/field` );
macro( RenameTypes = `compile/renametypes` );
macro( GETFILEPREFIX = `compile/fileprefix` );
macro( GENSYM = readlib(`tools/gensym`) );
macro( checkreadonly = `compile/checkreadonly` );
macro( WRITEOUT=`compile/writeout` );
macro( READIN=`compile/readin` );
macro( MapleExecutable = `compile/MapleExecutable` );
macro( writearrayutilities = `compile/write/arrayutilities` );
macro( arraydeclaration = `compile/arraydeclaration` );
macro( arraywrite = `compile/arraywrite` );
macro( arrayread = `compile/arrayread` );
macro( compileobjectonly = `compile/objectonly` );
macro( writefunction = `compile/writefunction` );
macro( writemain = `compile/writemain` );
macro( compileexecutable = `compile/executable` );



compile := proc(f::procedure) 
local
input,
output,
ret,
dir,
foo,
fileprefix,
opt,
mc, mo, fc, fo, ex,
par, params,
pre,
sep,
lan,
decls,
copts,
intrep,
body,
glos,
arrayargs,
needsmalloc,
COMPILER,
i,n,t,p;

global `compile/compile/array`, infolevel, `compile/compile/readonly`,
       `compile/ansi`;


  option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
  if not assigned( infolevel[compile] ) then
     infolevel[compile] := 0;
     userinfo(0,compile,
     `The compile command is presently only supported under Unix.`);
     userinfo(0,compile,
     `It will not run on a Macintosh or PC computer.`);
  fi;

  if not type(f,name) then # give the procedure the name foo
     userinfo(0,compile,`Using foo for the name of the procedure`,f);
     foo := f;
     RETURN( compile(foo,args[2..nargs]) )
  fi;

  if not type(f,symbol) then
     n := op(0,f);
     i := [op(f)];
     if not type(n,symbol) and type(i,list({integer,symbol})) then
        userinfo(0,compile,`Using foo for the name of the procedure`,f);
        foo := f;
        RETURN( compile(foo,args[2..nargs]) )
     fi;
     n := GENSYM( cat(n,op(i)) );
     assign(n,eval(f));
     RETURN( compile(n,args[2..nargs]) );
  fi;

  lan := 'C';
  COMPILER := '`cc`';
  `compile/ansi` := false;
  decls := [];
  copts := '`-lm`'; # Unix loader option
  dir := '`.`'; # Default directory
  opt := NULL;
  pre := double;
  sep := false;
  for i from 2 to nargs do
      if args[i]='optimized' then opt := opt, args[i]
      elif args[i]='ansi' then opt := opt, args[i]; `compile/ansi` := true;
      # elif type(args[i],identical(result)=type) then ret := rhs(args[i])
      elif type(args[i],identical(precision)=symbol) then
	   pre := rhs(args[i])
      elif type(args[i],identical(compiler_options)=symbol) then
           copts := ` `.(rhs(args[i]))
      elif type(args[i],identical(directory)=symbol) then
           dir := rhs(args[i]);
      elif type(args[i],identical(language)=symbol) then
           lan := rhs(args[i]);
      elif type(args[i],identical(declarations)=anything) then
	   decls := rhs(args[i]);
           if type(decls,`::`) then decls := [decls] fi;
           if not type(decls,{ list(`::`), set(`::`) }) then
              ERROR(`invalid declarations`,decls);
	   fi;
           decls := [declarations=decls];
      elif type(args[i],identical(compiler)=symbol) then
	   COMPILER := rhs(args[i]);
      elif type(args[i],identical(objectcode)) then sep := true;
      else ERROR(`unrecognized options`)
      fi;
  od;

  intrep := MAKEINTREP(f,op(decls));
  if intrep=FAIL then ERROR(`Unable to compile`) fi;

  glos := GET(intrep,'Globals');
  if nops(glos)>0 then ERROR(`Global variables not supported yet`); fi;

  body := GET(intrep,'StatSeq');
  if has(body,'Proc') then ERROR(`Nested procedures not supported yet`) fi;

  ret := GET(intrep,'Name');
  ret := rhs(op(1,ret));
  ret := RenameTypes(ret);
  if ret=numeric or ret=integer or ret='void' or # return types handled
     type(ret,function) and op(0,ret)='List' and
     member( op(nops(ret),ret), {integer,float,numeric} ) then
  else ERROR(`Unable to handle return type`,ret)
  fi;

  par := GET(intrep,Parameters);
  par := map(proc(x) op(1,x)::op(2,x) end, [op(par)]);
  params := map(proc(x) op(1,x) end,par);
  par := RenameTypes(par);

  # ARRAY : need to determine whether array parameters are read only or not
  arrayargs := NULL;
  for p in par do
      t := op(2,p);
      if t=numeric or t=integer then
      elif type(t,function) and op(0,t)='List' then
           arrayargs := arrayargs, op(1,p);
      else ERROR(`Unable to handle parameter type in`,p)
      fi;
  od:
  arrayargs := {arrayargs};


    # Determine if we can write in the current directory
    fileprefix := GETFILEPREFIX(f,dir);
    if fileprefix=FAIL then ERROR(`unable to open file for writing`) fi;

    fc := ``.fileprefix.f.`.c`;          # C code for function
    fo := ``.fileprefix.f.`.o`;          # Object code for function
    mc := ``.fileprefix.`main`.f.`.c`;   # C code for main program
    mo := ``.fileprefix.`main`.f.`.o`;   # Object code for main program
    input := ``.fileprefix.`input`.f;    # Data input file to C code
    output := ``.fileprefix.`output`.f;  # Data output file from C code
    ex := ``.fileprefix.`ex`.f;          # name of executable

    
    userinfo(0,compile,`Translating Maple code to `.lan);
    writefunction(f,fc,fo,lan,[opt],``,pre,decls);

    if sep then
       userinfo(0,compile,`Compiling `.source.` only.`);
       compileobjectonly(fc,fo,lan,COMPILER,copts);
       RETURN();
    fi;

    # Determine which arguments are read only by looking in the Maple code
    `compile/compile/array` := '`compile/compile/array`';
    `compile/compile/readonly` := '`compile/compile/readonly`';
    checkreadonly(body,arrayargs,params);

    needsmalloc := assigned(`compile/compile/array`);

    writemain(mc,f,input,output,par,ret,pre,needsmalloc,`compile/ansi`,lan);

    userinfo(0,compile,`Compiling and linking `.fc.` and `.mc.` .`);
    compileexecutable(fc,fo,mc,mo,ex,lan,COMPILER,copts,fileprefix);

    MapleExecutable(ex,input,output,par,ret,pre);

end:

checkreadonly := proc(f,A,P) local x; 
global `compile/compile/array`, `compile/compile/readonly`;
    option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
    if type(f,{name,numeric}) then
    elif type(f,'Assign'(indexed,anything)) and member(op([1,0],f),A) then
       `compile/compile/array`[ op([1,0],f) ] := true;
    elif type(f,'Assign'(symbol,anything)) and member(op(1,f),P) then
       `compile/compile/readonly`[ op(1,f) ] := false;
       ERROR(`Assigning to parameters not supported yet`,op(1,f));
    elif not type(f,function) then
    else for x in f do checkreadonly(x,A,P) od;
    fi
end:

writearrayutilities := proc(c,par,pre,reading)
local a,p,t,x,decl,written,ranges;
global `compile/compile/array`;
    option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
    for p in par do
        t := op(2,p);
        a := op(1,p);
        if type(t,function) and op(0,t)='List' then
           ranges := select(type,[op(t)],`..`);
           decl := select(type,{op(t)},symbol) intersect
		{integer,float,numeric};
           if nops(decl)=0 then decl := float; else decl := decl[1]; fi;
           if decl = numeric then decl := float; fi;
           if nops(ranges) = 1 then
              if decl=integer then
                 if reading and not assigned(written[1,integer,1]) then
                    PrintCCodeForReadingAVectorOfInt(c);
                    written[1,integer,1] := true;
                 fi;
                 if not reading and (`compile/compile/array`[a] = true) and
                    not assigned(written[1,integer,0]) then
                    PrintCCodeForWritingAVectorOfInt(c);
                    written[1,integer,1] := true;
                 fi;
              elif decl=float and pre=single then
                 if reading and not assigned(written[1,float,1]) then
                    PrintCCodeForReadingAVectorOfFloat(c);
                    written[1,float,1] := true;
                 fi;
                 if not reading and (`compile/compile/array`[a] = true) and
                    not assigned(written[1,float,0]) then
                    PrintCCodeForWritingAVectorOfFloat(c);
                    written[1,float,0] := true;
                 fi;
              elif decl=float and pre='double' then
                 if reading and not assigned(written[1,double,1]) then
                    PrintCCodeForReadingAVectorOfDouble(c);
                    written[1,double,1] := true;
                 fi;
                 if not reading and (`compile/compile/array`[a] = true) and
                    not assigned(written[1,double,0]) then
                    PrintCCodeForWritingAVectorOfDouble(c);
                    written[1,double,0] := true;
                 fi;
              else ERROR(`should not happen`);
              fi;
           elif nops(ranges) = 2 then
              if decl=integer then
                 if reading and not assigned(written[2,integer,1]) then
                    PrintCCodeForReadingAMatrixOfInt(c);
                    written[2,integer,1] := true;
                 fi;
                 if not reading and (`compile/compile/array`[a] = true) and
                    not assigned(written[2,integer,0]) then
                    PrintCCodeForWritingAMatrixOfInt(c);
                    written[2,integer,1] := true;
                 fi;
              elif decl=float and pre=single then
                 if reading and not assigned(written[2,float,1]) then
                    PrintCCodeForReadingAMatrixOfFloat(c);
                    written[2,float,1] := true;
                 fi;
                 if not reading and (`compile/compile/array`[a] = true) and
                    not assigned(written[2,float,0]) then
                    PrintCCodeForWritingAMatrixOfFloat(c);
                    written[2,float,0] := true;
                 fi;
              elif decl=float and pre='double' then
                 if reading and not assigned(written[2,double,1]) then
                    PrintCCodeForReadingAMatrixOfDouble(c);
                    written[2,double,1] := true;
                 fi;
                 if not reading and (`compile/compile/array`[a] = true) and
                    not assigned(written[2,double,0]) then
                    PrintCCodeForWritingAMatrixOfDouble(c);
                    written[2,double,0] := true;
                 fi;
              else ERROR(`should not happen`);
              fi;
           fi;
        fi;
    od;
end:

    
GETFILEPREFIX := proc(f,dir) local filename,status;
# Currently, have to use the ./ prefix for the current directory
# MBM: May/97
# In lines marked (*) below, the file is closed first because fopen
# currently gives an error on already opened files.  This might occur
# if the file is left open form a previous attempt to compile caused by
# an error during C code generation or a user interrupt.
option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if dir='`.`' then
    filename := `./`.f.`.c`;
    traperror(fclose(filename)); # (*)
    status := traperror( fopen(filename,WRITE) );
    if status <> lasterror then fclose(filename); RETURN( `./` ) fi;
    # Otherwise try /tmp/
    filename := `/tmp/`.f.`.c`;
    traperror(fclose(filename)); # (*)
    status := traperror( fopen(filename,WRITE) );
    if status <> lasterror then fclose(filename); RETURN( `/tmp/` ) fi;
    FAIL
elif dir='``' then
    filename := ``.f.`.c`;
    traperror(fclose(filename)); # (*)
    status := traperror( fopen(filename,WRITE) );
    if status <> lasterror then fclose(filename); RETURN( `` ) fi;
    FAIL
else # user specified directory
    filename := ``.dir.`/`.f.`.c`;
    traperror(fclose(filename)); # (*)
    status := traperror( fopen(filename,WRITE) );
    if status <> lasterror then fclose(filename); RETURN( ``.dir.`/` ) fi;
    FAIL;
fi;
end:

GET := proc(f,n::symbol,p::name) local i,x;
    option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
    for i to nops(f) do
        x := op(i,f);
        if type(x,function) and op(0,x)=n then
           if nargs=3 then p := i fi;
           RETURN(x)
        fi;
    od;
    FAIL
end:

RenameTypes := proc(X) local A,t;
   option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
   if type(X,symbol) then
      if member(X,'{float,real,rational,negative,positive,nonneg}') then numeric
      elif member(X,'{posint,negint,nonnegint,boolean}') then integer
      else X
      fi
   elif type(X,numeric) then X
   elif type(X,function) and op(0,X)=array then
	A := seq(procname(t),t=X);
        # Patch array declaration if it has no entry type
        if type([A],'list'(range)) then 'List'(A,numeric) else 'List'(A) fi;
   else map(procname,X)
   fi
end:

MapleExecutable := proc(ex,input,output,par,ret,pre)
local p,pars,rspec,rpars,dpar,special,n,arrayparams,t,retfmt,
      listout, spec_ranges, spec_type, spec_decl, spec_read;

#special := type(ret,List(integer..integer,identical(numeric)));
#if special then n := op(1,ret); n := rhs(n)-lhs(n)+1; fi;
#special := false;

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
listout := evalb( type(ret,function) and op(0,ret)='List' );
if listout then
    spec_ranges := select(type,[op(ret)],range);
    spec_type := op(nops(ret),ret);
    if spec_type=integer then spec_decl := `%d` else spec_decl := `%e` fi;
    spec_read := [ spec_decl, `result/value`, spec_ranges ];
else
    if ret=integer then retfmt := `%d`
    elif ret=numeric or ret=float then retfmt := `%e`
    elif ret='void' then
    else ERROR(`unable to handle return type`,ret);
    fi;
    spec_read := NULL;
fi;

  pars := map(proc(x,pre) local a,t,e,r,w;
                 a := op(1,x);
                 t := op(2,x);
                 if t=integer then `%d\n` = a
                 elif t=numeric or t=float then
                    if pre=single
                    then `%.7e\n` = a
                    else `%.16e\n` = a
                    fi
                 # ARRAY cases
                 elif type(t,function) and op(0,t)='List' then
                    w := assigned(`compile/compile/array`[a]);
                    e := op(nops(t),t);
                    r := select(type,[op(t)],range);
                    if e=integer then `%d\n` = [a,op(r),w];
                    elif e=numeric or e=float then
                         if pre=single
                         then `%.7e\n` = [a,op(r),w];
                         else `%.16e\n` = [a,op(r),w];
                         fi
                    fi;
                 else ERROR(`should not happen`);
                 fi
              end, par, pre);

  dpar := map(proc(x) local a,t;
      a := op(1,x);
      t := op(2,x);
      if type(t,function) and op(0,t)='List' then
         # Leave array bounds checking to inside code for now
         a::'array'
      else a::t
      fi
  end,par):

  arrayparams := false;
  for p in par while not arrayparams do
      t := op(2,p);
      if type(t,function) and op(0,t)='List' then arrayparams := true; fi;
  od;

     rpars := map(proc(x,pre) local a,t,e,r,w;
                 a := op(1,x);
                 t := op(2,x);
                 if t=integer or t=float or t=numeric then
                 elif type(t,function) and op(0,t)='List' then
                    w := assigned(`compile/compile/array`[a]);
                    if w then
                       e := op(nops(t),t);
                       r := select(type,[op(t)],range);
                       if e=integer then [`%d`, a, r];
                       elif e=numeric or e=float then [`%e`, a, r];
                       fi;
                    else
                    fi;
                 else ERROR(`should not happen`);
                 fi
              end, par, pre);

if not arrayparams and nops(par)=1 and not listout and not ret='void' then
    # Simple function of x
    subs( {_EXECUTABLE=ex,
           _SINGLEDIGITS=7,
           _DOUBLEDIGITS=16,
           _PARAM=op(1,op(par)),
           _TYPE=op(2,op(par)),
           _PARFMT=op(pars[1]),
           _INPUT=input,
           _OUTPUT=output,
           _SINGLE=evalb(pre='single'),
           _RETFMT=retfmt,
           _RETURN=ret},
      proc(_PARAM::_TYPE) local `result/value`;
         if _SINGLE then Digits := _SINGLEDIGITS;
		    else Digits := _DOUBLEDIGITS; fi;
         fprintf(_INPUT,_PARFMT);
         fclose(_INPUT);
         if system(_EXECUTABLE) <> 0 then ERROR(`execution failure`) fi;
         `result/value` := fscanf(_OUTPUT,_RETFMT);
         fclose(_OUTPUT);
         if not type(`result/value`,[_RETURN]) then
		ERROR(`data output error`) fi;
         `result/value`[1]
      end )
#  elif not arrayparams and nops(par)=1 and special then
elif false then
    subs( {_EXECUTABLE=ex,
           _SINGLEPRECISION=7,
           _DOUBLEPRECISION=16,
           _PARAM=op(1,op(par)),
           _TYPE=op(2,op(par)),
           _INPUT=input,
           _OUTPUT=output,
           _SINGLE=evalb(pre=single),
           _N=n,
           _FORMAT=cat(`%e`$n),
           _RETURN=op(2,ret)},
      proc(_PARAM::_TYPE) local `result/value`;
         if _SINGLE
         then Digits := _SINGLEPRECISION; fprintf(_INPUT,`%.7e\n`,x);
         else Digits := _DOUBLEPRECISION; fprintf(_INPUT,`%.16e\n`,x);
         fi;
         fclose(_INPUT);
         if system(_EXECUTABLE) <> 0 then ERROR(`execution failure`) fi;
         `result/value` := fscanf(_OUTPUT,_FORMAT);
         fclose(_OUTPUT);
         if not type(`result/value`,[_RETURN]) then
	    ERROR(`data output error`) fi;
         array(`result/value`);
      end )
elif not arrayparams and not listout and not ret='void' and nops(pars)<20 then
    # Simple function of n values, n<20
    subs( {_EXECUTABLE=ex,
           _SINGLEDIGITS=7,
           _DOUBLEDIGITS=16,
           _PARAMS=op(par),
           _WRITING=pars,
           _PARFMT=cat(seq(op(1,t),t=pars)),
           _PARNAMES=seq(op(2,t),t=pars),
           _INPUT=input,
           _SINGLE=evalb(pre=single),
           _OUTPUT=output,
           _RETFMT=retfmt,
           _RETURN=ret},
      proc(_PARAMS) local `result/value`;
         if _SINGLE
         then Digits := _SINGLEDIGITS;
         else Digits := _DOUBLEDIGITS;
         fi;
         fprintf(_INPUT,_PARFMT,_PARNAMES);
         fclose(_INPUT);
         if system(_EXECUTABLE) <> 0 then ERROR(`execution failure`) fi;
         `result/value` := fscanf(_OUTPUT,_RETFMT);
         fclose(_OUTPUT);
         if not type(`result/value`,[_RETURN]) then
	    ERROR(`data output error`) fi;
         `result/value`[1]
      end )
#  elif not arrayparams and special then
elif false then
    subs( {_EXECUTABLE=ex,
           _SINGLEPRECISION=7,
           _DOUBLEPRECISION=16,
           _PARAMS=op(par),
           _WRITING=pars,
           _INPUT=input,
           _SINGLE=evalb(pre=single),
           _OUTPUT=output,
           _N=n,
           _FORMAT=cat(`%e`$n),
           _RETURN=op(2,ret)},
      proc(_PARAMS) local `result/value`;
         if _SINGLE
         then Digits := _SINGLEPRECISION;
         else Digits := _DOUBLEPRECISION;
         fi;
         for `result/value` in _WRITING do
	     fprintf(_INPUT,op(`result/value`)); od;
         fclose(_INPUT);
         if system(_EXECUTABLE) <> 0 then ERROR(`execution failure`) fi;
         `result/value` := fscanf(_OUTPUT,_FORMAT);
         fclose(_OUTPUT);
         if not type(`result/value`,'list'(_RETURN)) then
	    ERROR(`data output error`) fi;
         array(`result/value`);
      end )
  # General cases: split into 1 and >1 parameters
  elif nops(pars) > 1 then
    subs( {_EXECUTABLE=ex,
           _SINGLEDIGITS=7,
           _DOUBLEDIGITS=16,
           _PARAMS=op(dpar),
           _WRITING=pars,
           _READING=[op(rpars),spec_read],
           _INPUT=input,
           _SINGLE=evalb(pre=single),
           _OUTPUT=output,
           _SPECIAL=listout,
           _INDEX = `compile/index`,
           _RANGES=op(spec_ranges),
           _FUNCTION=evalb(not ret='void' and not listout),
           _RESULT=`result/value`,
           _RETFMT=retfmt,
           _RETURN=ret},
      proc(_PARAMS) local _INDEX, _RESULT;
         if _SINGLE
         then Digits := _SINGLEDIGITS;
         else Digits := _DOUBLEDIGITS;
         fi;
         if _SPECIAL then _RESULT := array(_RANGES); fi;
         for _INDEX in _WRITING do WRITEOUT(_INPUT,op(_INDEX)); od;
         fclose(_INPUT);
         if system(_EXECUTABLE) <> 0 then ERROR(`execution failure`) fi;
         for _INDEX in _READING do READIN(_OUTPUT,op(_INDEX)); od;
  	 if _SPECIAL then
         elif _FUNCTION then
            _RESULT := fscanf(_OUTPUT,_RETFMT);
            if not type(_RESULT,[_RETURN]) then ERROR(`data output error`) fi;
         fi;
         fclose(_OUTPUT);
         if _SPECIAL then eval(_RESULT)
         elif _FUNCTION then _RESULT[1]
         else NULL
         fi;
      end )
  elif nops(pars) = 1 then # General case with one input
    subs( {_EXECUTABLE=ex,
           _SINGLEDIGITS=7,
           _DOUBLEDIGITS=16,
           _PARAM=op(1,op(dpar)),
           _TYPE=op(2,op(dpar)),
           _WRITING=pars,
           _READING=[op(rpars),spec_read],
           _INPUT=input,
           _SINGLE=evalb(pre=single),
           _OUTPUT=output,
           _SPECIAL=listout,
           _RANGES=op(spec_ranges),
           _FUNCTION=evalb(ret<>'void'),
           _RESULT=`result/value`,
           _INDEX=`compile/index`,
           _RETFMT=retfmt,
           _RETURN=ret},
      proc(_PARAM::_TYPE) local _INDEX, _RESULT;
         if _SINGLE
         then Digits := _SINGLEDIGITS;
         else Digits := _DOUBLEDIGITS;
         fi;
         if _SPECIAL then _RESULT := array(_RANGES); fi;
         for _INDEX in _WRITING do WRITEOUT(_INPUT,op(_INDEX)); od;
         fclose(_INPUT);
         if system(_EXECUTABLE) <> 0 then ERROR(`execution failure`) fi;
         for _INDEX in _READING do READIN(_OUTPUT,op(_INDEX)); od;
         if _SPECIAL then
         elif _FUNCTION then
            _RESULT := fscanf(_OUTPUT,_RETFMT);
            if not type(_RESULT,[_RETURN]) then
               ERROR(`data output error`) fi;
         fi;
         fclose(_OUTPUT);
         if _SPECIAL then eval(_RESULT)
         elif _FUNCTION then _RESULT[1]
         else NULL
         fi;
      end )
  fi;
end:


WRITEOUT := proc(input,f,v) local a,i,j,l,m,r,x;
    option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
    if not type(v,list) then
       fprintf(input,f,v);
    else
       a := v[1];
       r := select(type,v,`..`);
       if not type(a,'array'(op(r))) then
	  ERROR(`input should be of type`,'array'(op(r))) fi;
       if nops(r)=1 then
          for i from lhs(r[1]) to rhs(r[1]) do
             x := a[i];
             if x=undefined or not(assigned(a[i])) then x := 0 fi;
             fprintf(input,f,x);
          od;
       elif nops(r)=2 then
          l := lhs(r[2]); m := rhs(r[2]);
          for i from lhs(r[1]) to rhs(r[1]) do
              for j from l to m do
                  x := a[i,j];
                  if x=undefined or not(assigned(a[i,j])) then x := 0 fi;
                  fprintf(input,f,x);
              od;
          od;
       else ERROR(`should not happen`);
       fi;
    fi;
end:

READIN := proc(output,fmt,a,r) local i,j,l,m,x;
    option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
    if nops(r)=1 then
       for i from lhs(r[1]) to rhs(r[1]) do
          x := fscanf(output,fmt);
          a[i] := x[1];
       od;
    elif nops(r)=2 then
       l := lhs(r[2]); m := rhs(r[2]);
       for i from lhs(r[1]) to rhs(r[1]) do
           for j from l to m do x := fscanf(output,fmt); a[i,j] := x[1]; od;
       od;
    else ERROR(`should not happen`);
    fi;
end:

compileobjectonly := proc( source, object, language, compiler, compileroptions )
    option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
    if language='C' then
       if system(``.compiler.` -c `.source.` -o `.object.` `.
                  compileroptions) <> 0 then
         ERROR(`Code failed to compile`)
       fi;
    # else Fortran
    fi;
end:

compileexecutable := proc(
    fsource,
    fobject,
    mainsource,
    mainobject,
    executable,
    language,
    compiler,
    compileroptions,
    fileprefix)
    local command;
    option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
    if language='C' then
       if fileprefix = '`./`' then # Compile together in the current directory
          command := ``.compiler.` -o `.
                  executable.` `.
                  fsource.` `.
                  mainsource.` `.
                  compileroptions;
          userinfo(1,compile,command);
          if system(command) <> 0 then ERROR(`Code failed to compile`) fi;
       else # compile them seperately and link
          command :=  ``.compiler.` -o `.fobject.` -c `.fsource.` `.
                         compileroptions;
          userinfo(1,compile,command);
          if system(command) <> 0 then ERROR(`Code failed to compile`) fi;
          command := ``.compiler.` -o `.mainobject.` -c `.mainsource.` `.
                        compileroptions;
          userinfo(1,compile,command);
          if system(command) <> 0 then ERROR(`Code failed to compile`) fi;
	  command := ``.compiler.` -o `.executable.` `.
                        fobject.` `.mainobject.` `.compileroptions;
          userinfo(1,compile,command);
          if system(command) <> 0 then ERROR(`Code failed to compile`) fi;
       fi;
    # else Fortran
    fi;
end:

writefunction := proc(
    function::procedure,
    sourcefile::symbol,
    objectfile::symbol,
    language::{identical(C),identical(Fortran)},
    translator_options::list,
    compiler_options::symbol,
    pre::{identical(single),identical(double)},
    decls
    )

    option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
    if language=C then
       fopen(sourcefile,WRITE); # clears the file
       fclose(sourcefile);
       readlib(C)(function,
                  filename=sourcefile,
                  precision=pre,
                  op(translator_options),
                  op(decls)
                 );
    elif language=Fortran then
       fopen(sourcefile,WRITE); # clears the file
       fclose(sourcefile);
       readlib(fortran)(function,
                  filename=sourcefile,
                  precision=single,
                  op(translator_options)
                 );
    fi;

end:

writemain := proc(c,f,input,output,par,ret,
   pre::{identical(single),identical(double)},needsmalloc,ansi,language)
local x, t,
   argseq,
   decl,
   fmt,
   spec_output,
   spec_type,
   spec_ranges,
   listout,
   nm,
   m,n, # dimension of array passed back
   N,   # total storage needed
   p, 
   dummy,
   indentation,
   special; # flag indicating array passed back as extra argument
global `compile/compile/array`;


option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
special := type(ret,List(integer..integer,identical(numeric)));
if special then n := op(1,ret); n := rhs(n)-lhs(n)+1; fi;
special := false;

listout := evalb( type(ret,function) and op(0,ret)='List' );
spec_output := NULL;
if listout then
   spec_ranges := select(type,[op(ret)],range);
   spec_type := op(nops(ret),ret);
   nm := not type(spec_ranges,list(integer..integer));
   spec_output := dummy=ret; `compile/compile/array`[dummy] := true;
fi;


###########################################################################
fopen(c,WRITE);
fprintf(c, `#include <stdio.h>\n`);

if needsmalloc or nm=true then fprintf(c, `#include <stdlib.h>\n`); fi;

# Write out vector and matrix reading and writing utilities
writearrayutilities(c,par,pre,true);
writearrayutilities(c,[op(par),spec_output],pre,false);

###########################################################################
# Write out main program
fprintf(c, `main()\n`);
fprintf(c, `{\n`);

# Declarations
for x in par do
   p := op(1,x);
   t := op(2,x);
   if t = integer then
      fprintf(c, `int `.(p).`;\n`);
   elif t=numeric or t=float or t=rational then
      if pre='single' then decl := `float `; else decl := `double `; fi;
      fprintf(c, ``.(decl).(p).`;\n`);
   # ARRAY cases
   elif type(t,function) and op(0,t)='List' then
       decl := arraydeclaration(p,[op(t)],pre);
       fprintf(c, `%s;\n`, decl);
   else ERROR(`type not implemented`,x);
   fi;
od;

if ret=integer then
   fprintf(c, `int RESULT, %s();\n`, f);
elif (ret=numeric or ret=float) then
   if pre=single then
      fprintf(c, `float RESULT, %s();\n`, f);
   else
      fprintf(c, `double RESULT, %s();\n`, f);
   fi;
elif ret='void' then
      fprintf(c, `void `.f.`();\n`);
elif special then
   fprintf(c, `int i;\n`);
   if pre=single then
      fprintf(c, `float RESULT[`.n.`];\n`);
      fprintf(c, `void `.f.`();\n`);
   else
      fprintf(c, `double RESULT[`.n.`];\n`);
      fprintf(c, `void `.f.`();\n`);
   fi;
elif listout then
   fprintf(c, `void `.f.`();\n`);
   if spec_type = integer then fprintf(c, `int `);
   elif spec_type = numeric or spec_type = float then
      if pre = single then fprintf(c, `float `) else fprintf(c, `double `) fi;
   else ERROR(`should not happen`)
   fi;
   if type(spec_ranges,[integer..integer]) then
      n := spec_ranges[1]; n := rhs(n)-lhs(n)+1;
      fprintf(c,`RESULT [%d];\n`,n);
   elif type(spec_ranges,[integer..integer,integer..integer]) then
      m := spec_ranges[1]; m := rhs(m)-lhs(m)+1;
      n := spec_ranges[2]; n := rhs(n)-lhs(n)+1;
      fprintf(c,`RESULT [%d][%d];\n`,m,n);
   elif nops(spec_ranges)=1 then
      n := spec_ranges[1]; n := rhs(n)-lhs(n)+1;
      fprintf(c,`*RESULT;\n`);
   elif nops(spec_ranges)=2 then
      m := spec_ranges[1]; m := rhs(m)-lhs(m)+1;
      n := spec_ranges[2]; n := rhs(n)-lhs(n)+1;
      fprintf(c,`*RESULT;\n`);
   fi;

else ERROR(`type not implemented`,ret)
fi;

fprintf(c, `FILE *fp, *fopen();\n`);

# Data input
fprintf(c, `   fp = fopen("`.input.`","r");\n`);
fprintf(c, `   if( fp==NULL ) exit(1);\n`);
for x in par do
   p := op(1,x);
   t := op(2,x);
   if t=integer then
      fprintf(c, `   fscanf(fp,"%%d",&`.(p).`);\n`);
   elif (t=numeric or t=float) and pre=single then
      fprintf(c, `   fscanf(fp,"%%e",&`.(p).`);\n`);
   elif (t=numeric or t=float) and pre=double then
      fprintf(c, `   fscanf(fp,"%%le",&`.(p).`);\n`);
   # Array cases
   elif type(t,function) and op(0,t)='List' then
      arrayread(c,p,[op(t)],pre);
   fi;
od;
fprintf(c, `   fclose(fp);\n`);

# Allocate storage for return array
if nm=true then
    if spec_type = integer then decl := 'int';
    elif pre = 'single' then decl := 'float';
    else decl := 'double';
    fi;
    # allocate storage
    if nops(spec_ranges)=1 then
       fprintf(c,`   RESULT = (%s *) malloc(sizeof(%s)*(%s));\n`,
       decl, decl, convert(n,string));
    else
       fprintf(c,`   RESULT = (%s *) malloc(sizeof(%s)*(%s)*(%s));\n`,
       decl, decl, convert(m,string), convert(n,string));
    fi;
fi;

# Initialization
#if special then fprintf(c, `   for(i=0; i<`.n.`; i++) RESULT[i] = 0;\n`); fi;

# Function executation
argseq := [seq(op(1,x),x=par)];
if special or listout then argseq := [op(argseq),'RESULT']; fi;
argseq := map(proc(x) `, `,x end,argseq);
argseq := argseq[2..nops(argseq)];

if ret='void' then
   # Subroutine call
   fprintf(c, cat(f,`(`,op(argseq),`);\n`) );
elif special or listout then
   # Subroutine call with array passed back
   fprintf(c, cat(`   `,f,`(`,op(argseq),`);\n`) );
else
   # Normal function call
   fprintf(c, cat(`   RESULT = `,f,`(`,op(argseq),`);\n`) );
fi;

# Data output
fprintf(c, `   fp = fopen("`.output.`","w");\n`);
fprintf(c, `   if( fp==NULL ) exit(1);\n`);
for x in par do
   p := op(1,x);
   t := op(2,x);
   if type(t,function) and op(0,t)='List' then
      if `compile/compile/array`[p]=true then arraywrite(c,p,[op(t)],pre); fi;
   fi;
od;
if special then
  if pre=single then fmt := `"%%.7e\\n"` else fmt := `"%%.16le\\n"` fi;
  fprintf(c, `   for( i=0; i<`.n.`; i++ ) fprintf(fp,`.fmt.`,RESULT[i]);\n`);
elif listout then
  if spec_type=integer and nops(spec_ranges)=1 then
     n := convert(n,string);
     fprintf(c,`   MapleWriteVectorOfInt(fp,RESULT,`.(n).`);\n`);
  elif spec_type=integer and nops(spec_ranges)=2 then
     m := convert(m,string); n := convert(n,string);
     fprintf(c,`   MapleWriteMatrixOfInt(fp,RESULT,%s,%s);\n`,m,n);
  elif spec_type=numeric and nops(spec_ranges)=1 then
     n := convert(n,string);
     if pre='single' then
        fprintf(c,`   MapleWriteVectorOfFloat(fp,RESULT,`.(n).`);\n`);
     else
        fprintf(c,`   MapleWriteVectorOfDouble(fp,RESULT,`.(n).`);\n`);
     fi;
  elif spec_type=numeric and nops(spec_ranges)=2 then
     m := convert(m,string); n := convert(n,string);
     if pre='single' then
        fprintf(c,`   MapleWriteMatrixOfFloat(fp,RESULT,%s,%s);\n`,m,n);
     else
        fprintf(c,`   MapleWriteMatrixOfDouble(fp,RESULT,%s,%s);\n`,m,n);
     fi;
  fi;
elif ret='void' then # do nothing
else
  if ret=integer then fmt := `"%%d\\n"`
  elif pre=single then fmt := `"%%.7e\\n"`
  else fmt := `"%%.16le\\n"`
  fi;
  fprintf(c, `   fprintf(fp,`.fmt.`,RESULT);\n`);
fi;
fprintf(c, `   fclose(fp);\n`);

# Return
fprintf(c, `   exit(0);\n`);
fprintf(c, `}\n`);
fclose(c);

end:

arraydeclaration := proc(p::symbol,t,pre)
local ranges, entype, decl, r;
    option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
    ranges := select(type,t,range);
    entype := {op(select(type,t,symbol))} intersect {integer,float,numeric};
    if entype={} then entype := float; else entype := entype[1]; fi;
    if entype=integer then decl := `int `;
    elif pre=single then decl := `float `;
    else decl := `double `;
    fi;
    if type(ranges,list(integer..integer)) then
       decl := cat(decl,p);
       for r in ranges do
           decl := cat( decl, '`[`', rhs(r)-lhs(r)+1, '`]`' );
       od;
    else 
       decl := cat( decl, '`*`', p );
    fi;
    decl
end:


arrayread := proc(c,a,t,p)
local ranges, entype, f, r, m, n, decl;
    option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
    ranges := select(type,t,range);
    entype := {op(select(type,t,symbol))} intersect {integer,float,numeric};
    if entype={} then entype := float; else entype := entype[1]; fi;
    if entype = integer then decl := 'int';
    elif p = 'single' then decl := 'float';
    else decl := 'double'; 
    fi;
    n := mul( rhs(r)-lhs(r)+1, r=ranges );
    n := convert(n,string);
    if type(ranges,list(integer..integer)) then
    else
       m := rhs(ranges[1])-lhs(ranges[1])+1;
       m := convert(m,string);
       # allocate storage
       # f := cat(a, ` = `, `malloc(`, `sizeof(`, decl, `)*`, n, `);`);
       # fprintf(c,`%s;\n`,f);
       # fprintf(c,`%s = malloc(sizeof(%s)*(%s));\n`,a,decl,n);
       if nops(ranges)=1 then
          fprintf(c,`%s = (%s *) malloc(sizeof(%s)*(%s));\n`,a,decl,decl,n);
       elif nops(ranges)=2 then
          n := rhs(ranges[2])-lhs(ranges[2])+1;
          n := convert(n,string);
          fprintf(c,`%s = (%s *) malloc(sizeof(%s)*(%s)*(%s));\n`,
		  a,decl,decl,m,n);
       fi;
    fi;
    if entype=integer then
          if nops(ranges)=1 then
             f := 'MapleReadVectorOfInt';
          elif nops(ranges)=2 then
             f := 'MapleReadMatrixOfInt';
	  else ERROR(`should not happen`);
 	  fi;
    elif entype=float or entype=numeric then
          if nops(ranges)=1 and p=single then
             f := 'MapleReadVectorOfFloat';
          elif nops(ranges)=1 then
             f := 'MapleReadVectorOfDouble';
          elif nops(ranges)=2 and p=single then
             f := 'MapleReadMatrixOfFloat';
          elif nops(ranges)=2 then
             f := 'MapleReadMatrixOfDouble';
	  else ERROR(`should not happen`);
 	  fi;
    fi;
    m := rhs(ranges[1])-lhs(ranges[1])+1;
    m := convert(m,string);
    if nops(ranges)=1 then
         #f := cat(f, `(`, 'fp', `,`, a, `,`, m, `)`);
         fprintf(c, `%s(fp,%s,%s);\n`, f,a,m );
    elif nops(ranges)=2 then
         n := rhs(ranges[2])-lhs(ranges[2])+1;
         n := convert(n,string);
         # f := cat(f, `(`, 'fp', `,`, a, `,`, m, `,`, n, `)`);
         fprintf(c, `%s(fp,%s,%s,%s);\n`, f,a,m,n );
    else ERROR(`should not happen`)
    fi;
    #fprintf(c, `%s;\n`, f);
end:

arraywrite := proc(c,a,t,p)
local ranges, entype, f, r, m, n;
    option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
    ranges := select(type,t,range);
    entype := {op(select(type,t,symbol))} intersect {integer,float,numeric};
    if entype={} then entype := float; else entype := entype[1]; fi;
    if entype=integer then
          if nops(ranges)=1 then
             f := 'MapleWriteVectorOfInt';
          elif nops(ranges)=2 then
             f := 'MapleWriteMatrixOfInt';
          else ERROR(`should not happen`);
          fi;
    elif entype=float or entype=numeric then
          if nops(ranges)=1 and p=single then
             f := 'MapleWriteVectorOfFloat';
          elif nops(ranges)=1 then
             f := 'MapleWriteVectorOfDouble';
          elif nops(ranges)=2 and p=single then
             f := 'MapleWriteMatrixOfFloat';
          elif nops(ranges)=2 then
             f := 'MapleWriteMatrixOfDouble';
          else ERROR(`should not happen`);
          fi;
    fi;
    m := rhs(ranges[1])-lhs(ranges[1])+1;
    m := convert(m,string);
    if nops(ranges)=1 then
       fprintf(c, `   %s(fp,%s,%s);\n`, f,a,m );
    elif nops(ranges)=2 then
       n := rhs(ranges[2])-lhs(ranges[2])+1;
       n := convert(n,string);
       fprintf(c, `   %s(fp,%s,%s,%s);\n`, f,a,m,n );
    else ERROR(`should not happen`)
    fi;
    #f := cat(f, `(`, 'fp', `,`, a, `,`, m, `,`, n, `)`);
    #fprintf(c, `%s;\n`, f);
end:

####################################################################
# Utilitities for reading and writing vectors and matrices         #
# Allowed entry types: int, float, double                          #
####################################################################
# Reading Vectors                                                  #
####################################################################
PrintCCodeForReadingAVectorOfDouble := proc(CodeFile)

#void MapleReadVectorOfDouble(f,v,n)
#double *v;
#int n;
#FILE *f;
#{ int i;
#for( i=0; i<n; i++ ) fscanf(f,"%le",v+i);
#return; }

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if `compile/ansi` = true then
fprintf(CodeFile,
`void MapleReadVectorOfDouble(FILE *f, double *v, int n)\n`);
else
fprintf(CodeFile,`void MapleReadVectorOfDouble(f,v,n)\n`);
fprintf(CodeFile,`FILE *f; double *v; int n;\n`);
fi;
fprintf(CodeFile,`{ int i;\n`);
fprintf(CodeFile,`for( i=0; i<n; i++ ) fscanf(f,"%%le",&v[i]);\n`);
fprintf(CodeFile,`return; }\n`);

end:

PrintCCodeForReadingAVectorOfInt := proc(CodeFile)

#void MapleReadVectorOfInt(f,v,n)
#int *v;
#int n;
#FILE *f;
#{ int i;
#for( i=0; i<n; i++ ) fscanf(f,"%d",v+i);
#return; }

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if `compile/ansi` = true then
fprintf(CodeFile,
`void MapleReadVectorOfInt(FILE *f, int *v, int n)\n`);
else
fprintf(CodeFile,`void MapleReadVectorOfInt(f,v,n)\n`);
fprintf(CodeFile,`FILE *f; int *v; int n;\n`);
fi;
fprintf(CodeFile,`{ int i;\n`);
fprintf(CodeFile,`for( i=0; i<n; i++ ) fscanf(f,"%%d",&v[i]);\n`);
fprintf(CodeFile,`return; }\n`);

end:

PrintCCodeForReadingAVectorOfFloat := proc(CodeFile)

#void MapleReadVectorOfFloat(f,v,n)
#float *v;
#int n;
#FILE *f;
#{ int i;
#for( i=0; i<n; i++ ) fscanf(f,"%e",v+i);
#return; }

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if `compile/ansi`=true then
fprintf(CodeFile,
`void MapleReadVectorOfFloat(FILE *f, float *v, int n)\n`);
else
fprintf(CodeFile,`void MapleReadVectorOfFloat(f,v,n)\n`);
fprintf(CodeFile,`FILE *f; float *v; int n;\n`);
fi;
fprintf(CodeFile,`{ int i;\n`);
fprintf(CodeFile,`for( i=0; i<n; i++ ) fscanf(f,"%%e",&v[i]);\n`);
fprintf(CodeFile,`return; }\n`);

end:

####################################################################
# Writing Vectors                                                  #
####################################################################
PrintCCodeForWritingAVectorOfDouble := proc(CodeFile)

#void MapleWriteVectorOfDouble(f,v,n)
#double *v;
#int n;
#FILE *f;
#{ int i;
#for( i=0; i<n; i++ ) fprintf(f,"%.16le\n",v[i]);
#return; }

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if `compile/ansi`=true then
fprintf(CodeFile,
`void MapleWriteVectorOfDouble(FILE *f,double *v,int n)\n`);
else
fprintf(CodeFile,`void MapleWriteVectorOfDouble(f,v,n)\n`);
fprintf(CodeFile,`FILE *f; double *v; int n;\n`);
fi;
fprintf(CodeFile,`{ int i;\n`);
fprintf(CodeFile,`for( i=0; i<n; i++ ) fprintf(f,"%%.16le\\n", v[i]);\n`);
fprintf(CodeFile,`return; }\n`);

end:

PrintCCodeForWritingAVectorOfFloat := proc(CodeFile)

#void MapleWriteVectorOfFloat(f,v,n)
#float *v;
#int n;
#FILE *f;
#{ int i;
#for( i=0; i<n; i++ ) fprintf(f,"%.7e\n",v[i]);
#return; }

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if `compile/ansi`=true then
fprintf(CodeFile,
`void MapleWriteVectorOfFloat(FILE *f,float *v,int n)\n`);
else
fprintf(CodeFile,`void MapleWriteVectorOfFloat(f,v,n)\n`);
fprintf(CodeFile,`FILE *f; float *v; int n;\n`);
fi;
fprintf(CodeFile,`{ int i;\n`);
fprintf(CodeFile,`for( i=0; i<n; i++ ) fprintf(f,"%%.7e\\n", v[i]);\n`);
fprintf(CodeFile,`return; }\n`);

end:

PrintCCodeForWritingAVectorOfInt := proc(CodeFile)

#void MapleWriteVectorOfInt(f,v,n)
#int *v;
#int n;
#FILE *f;
#{ int i;
#for( i=0; i<n; i++ ) fprintf(f,"%d\n",v[i]);
#return; }

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if `compile/ansi`=true then
fprintf(CodeFile,
`void MapleWriteVectorOfInt(FILE *f,int *v,int n)\n`);
else
fprintf(CodeFile,`void MapleWriteVectorOfInt(f,v,n)\n`);
fprintf(CodeFile,`FILE *f; int *v; int n;\n`);
fi;
fprintf(CodeFile,`{ int i;\n`);
fprintf(CodeFile,`for( i=0; i<n; i++ ) fprintf(f,"%%d\\n", v[i]);\n`);
fprintf(CodeFile,`return; }\n`);

end:


####################################################################
# Writing Matrices                                                 #
####################################################################
PrintCCodeForWritingAMatrixOfDouble := proc(CodeFile)

#void MapleWriteMatrixOfDouble(f,A,m,n)
#FILE *f;
#double *A;
#int m,n;
#{ int i,j;
#for(i=0; i<m; i++) for(j=0; j<n; j++) { fprintf(f,"%.16le\n",A[0]); A++; }
#return; }

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if `compile/ansi`=true then
fprintf(CodeFile,
`void MapleWriteMatrixOfDouble(FILE *f,double *A,int m,int n)\n`);
else
fprintf(CodeFile,`void MapleWriteMatrixOfDouble(f,A,m,n)\n`);
fprintf(CodeFile,`FILE *f; double *A; int m,n;\n`);
fi;
fprintf(CodeFile,`{ int i,j;\n`);
fprintf(CodeFile,`for(i=0; i<m; i++ ) for( j=0; j<n; j++)`);
fprintf(CodeFile,` { fprintf(f,"%%.16le\\n",A[0]); A++; }\n`);
fprintf(CodeFile,`return; }\n`);

end:

PrintCCodeForWritingAMatrixOfFloat := proc(CodeFile)

#void MapleWriteMatrixOfFloat(f,A,m,n)
#FILE *f;
#float *A;
#int m,n;
#{ int i,j; 
#for(i=0; i<m; i++) for(j=0; j<n; j++) { fprintf(f,"%.7e\n",A[0]); A++; }    
#return; }

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if `compile/ansi`=true then
fprintf(CodeFile,
`void MapleWriteMatrixOfFloat(FILE *f,float *A,int m,int n)\n`);
else
fprintf(CodeFile,`void MapleWriteMatrixOfFloat(f,A,m,n)\n`);
fprintf(CodeFile,`FILE *f; float *A; int m,n;\n`);
fi;
fprintf(CodeFile,`{ int i,j;\n`);
fprintf(CodeFile,`for(i=0; i<m; i++ ) for( j=0; j<n; j++)`);    
fprintf(CodeFile,` { fprintf(f,"%%.8e\\n",A[0]); A++; }\n`);
fprintf(CodeFile,`return; }\n`);

end:  


PrintCCodeForWritingAMatrixOfInt := proc(CodeFile)

#void MapleWriteMatrixOfInt(f,A,m,n)
#FILE *f;
#int *A;
#int m,n;
#{ int i,j; 
#for(i=0; i<m; i++) for(j=0; j<n; j++) { fprintf(f,"%d\n",A[0]); A++; }    
#return; }

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if `compile/ansi`=true then
fprintf(CodeFile,
`void MapleWriteMatrixOfInt(FILE *f,int *A,int m,int n)\n`);
else
fprintf(CodeFile,`void MapleWriteMatrixOfInt(f,A,m,n)\n`);
fprintf(CodeFile,`FILE *f; int *A; int m,n;\n`);
fi;
fprintf(CodeFile,`{ int i,j;\n`);
fprintf(CodeFile,`for(i=0; i<m; i++ ) for( j=0; j<n; j++)`);    
fprintf(CodeFile,` { fprintf(f,"%%d\\n",A[0]); A++; }\n`);
fprintf(CodeFile,`return; }\n`);

end:  

####################################################################
# Reading Matrices                                                 #
####################################################################
PrintCCodeForReadingAMatrixOfDouble := proc(CodeFile)

#void MapleReadMatrixOfDouble(f,A,m,n)
#FILE *f;
#double *A;
#int m,n;
#{ int i,j,o; static double x;
#for(i=0; i<m; i++) for(j=0; j<n; j++) { fscanf(f,"%le",&x); A[0] = x; A++; }
#return; }

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if `compile/ansi` = true then
fprintf(CodeFile,
`void MapleReadMatrixOfDouble(FILE *f,double *A,int m,int n)\n`);
else
fprintf(CodeFile,`void MapleReadMatrixOfDouble(f,A,m,n)\n`);
fprintf(CodeFile,`FILE *f; double *A; int m,n;\n`);
fi;
fprintf(CodeFile,`{ int i,j; static double x;\n`);
fprintf(CodeFile,`for(i=0; i<m; i++) for(j=0; j<n; j++)`);
fprintf(CodeFile,` { fscanf(f,"%%le",&x); A[0] = x; A++; }\n`);
fprintf(CodeFile,`return; }\n`);

end:

PrintCCodeForReadingAMatrixOfFloat := proc(CodeFile)

#void MapleReadMatrixOfFloat(f,A,m,n)
#FILE *f;
#float *A;
#int m,n;
#{ int i,j,o; static float x;
#for(i=0; i<m; i++) for(j=0; j<n; j++) { fscanf(f,"%e",&x); A[0] = x; A++; }
#return; }

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if `compile/ansi`=true then
fprintf(CodeFile,
`void MapleReadMatrixOfFloat(FILE *f,float *A,int m,int n)\n`);
else
fprintf(CodeFile,`void MapleReadMatrixOfFloat(f,A,m,n)\n`);
fprintf(CodeFile,`FILE *f; float *A; int m,n;\n`);
fi;
fprintf(CodeFile,`{ int i,j; static float x;\n`);
fprintf(CodeFile,`for(i=0; i<m; i++) for(j=0; j<n; j++)`);
fprintf(CodeFile,` { fscanf(f,"%%e",&x); A[0] = x; A++; }\n`);
fprintf(CodeFile,`return; }\n`);

end:


PrintCCodeForReadingAMatrixOfInt := proc(CodeFile)

#void MapleReadMatrixOfInt(f,A,m,n)
#FILE *f;
#int *A;
#int m,n;
#{ int i,j,o; static int x;
#for(i=0; i<m; i++) for(j=0; j<n; j++) { fscanf(f,"%d",&x); A[0] = x; A++; }
#return; }

option `Copyright (c) 1997 Waterloo Maple Inc. All rights reserved.`;
if `compile/ansi` = true then
fprintf(CodeFile,
`void MapleReadMatrixOfInt(FILE *f, int *A, int m, int n)\n`);
else
fprintf(CodeFile,`void MapleReadMatrixOfInt(f,A,m,n)\n`);
fprintf(CodeFile,`FILE *f; int *A; int m,n;\n`);
fi;
fprintf(CodeFile,`{ int i,j; static int x;\n`);
fprintf(CodeFile,`for(i=0; i<m; i++) for(j=0; j<n; j++)`);
fprintf(CodeFile,` { fscanf(f,"%%d",&x); A[0] = x; A++; }\n`); 
fprintf(CodeFile,`return; }\n`);

end:

read "compile.msl":
#savelib('_Share',\
'compile', 'checkreadonly', 'writearrayutilities', 'GETFILEPREFIX',\
'GET', 'RenameTypes', 'MapleExecutable', 'WRITEOUT', 'READIN',\
'compileobjectonly', 'compileexecutable', 'writefunction', 'writemain',\
'arraydeclaration', 'arrayread', 'arraywrite',\
'PrintCCodeForReadingAVectorOfDouble',\
'PrintCCodeForReadingAVectorOfInt',\
'PrintCCodeForReadingAVectorOfFloat',\
'PrintCCodeForWritingAVectorOfDouble',\
'PrintCCodeForWritingAVectorOfFloat',\
'PrintCCodeForWritingAVectorOfInt',\
'PrintCCodeForWritingAMatrixOfDouble',\
'PrintCCodeForWritingAMatrixOfFloat',\
'PrintCCodeForWritingAMatrixOfInt',\
'PrintCCodeForReadingAMatrixOfDouble',\
'PrintCCodeForReadingAMatrixOfFloat',\
'PrintCCodeForReadingAMatrixOfInt',\
`compile.m` );
