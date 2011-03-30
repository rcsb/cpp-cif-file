#
# SConscript for cif-file-v1.0
# Created: Aug 16, 2006 - Jdw
# Updated: Aug 23, 2006 - Jdw
#          Add aggregated library..
# Updated: Mar 30, 2011 jdw clone environment
#
import os.path,glob
Import('env')
env=env.Clone()
#
#if (len(env.subst('$MYDEBUG')) > 0):
#	dict = env.Dictionary()
#	for k,v in dict.items():
#		print  k, " = ", str(v)
#
libName = 'cif-file'


libSrcList =['src/CifFile.C',
			 'src/DicFile.C',
                         'src/ParentChild.C',
                         'src/CifParentChild.C',
                         'src/CifDataInfo.C',
                         'src/CifExcept.C']
libObjList = [s.replace('.C','.o') for s in libSrcList]
libIncList =['include/CifFile.h',
			 'include/DicFile.h',
                         'include/ParentChild.h',
                         'include/CifParentChild.h',
            		 'include/CifDataInfo.h',
                         'include/CifExcept.h']

#
#oPath = os.path.join(env.subst('$MY_OBJ_INSTALL_PATH'),'*.o')
#depObjs=glob.glob(oPath)
#
aggLibName='cif-all'


aggObjs = [ 'MmcifToXml.o','XmlWriter.o','XsdWriter.o','PdbMlSchema.o','PdbMlWriter.o',
 		'CifDataInfo.o','CifExcept.o', 'CifParentChild.o', 'ParentChild.o', 'CifFile.o',
	    'CifFileUtil.o', 'CifCorrector.o',	    
		   'CifFileReadDef.o',
		   'CifParser.o',
		   'CifParserBase.o',
		   'CifScanner.o',
		   'CifScannerBase.o',
		   'DICParser.o',
		   'DICParserBase.o',
		   'DICScanner.o',
		   'DICScannerBase.o',
		   'DicFile.o',
		   'ISTable.o',
		   'ITTable.o',
		   'TTable.o',
		   'TableFile.o',
		   'BlockIO.o',
	           'GenCont.o',
                   'DataInfo.o',
	           'RcsbFile.o',
	           'RcsbPlatform.o',
		   'CifString.o',
		   'Serializer.o',
		   'GenString.o',
		   'Exceptions.o',
#		   'ndb_misclib.o',
#		   'ndb_string_util.o',
#		   'ndb_sysutil.o',
#		   'partime.o',
		   'regcomp.o',
		   'regerror.o',
		   'regexec.o',
		   'regfree.o']
aggObjList = [os.path.join(env.subst('$MY_OBJ_INSTALL_PATH'),o) for o in aggObjs]

#
myLib=env.Library(libName,libSrcList)
#
myAggLib=env.Library(aggLibName,aggObjList)
#
env.Install(env.subst('$MY_LIB_INSTALL_PATH'),myLib)
env.Install(env.subst('$MY_LIB_INSTALL_PATH'),myAggLib)
env.Alias('install-lib',env.subst('$MY_LIB_INSTALL_PATH'))
#
env.Install(env.subst('$MY_INCLUDE_INSTALL_PATH'),libIncList)
env.Alias('install-include',env.subst('$MY_INCLUDE_INSTALL_PATH'))
#
env.Install(env.subst('$MY_OBJ_INSTALL_PATH'),libObjList)
env.Alias('install-obj',env.subst('$MY_OBJ_INSTALL_PATH'))
#
env.Default('install-lib','install-include','install-obj')
#
#
