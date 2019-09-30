################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################

#!/usr/bin/perl -w
#use strict;
use POSIX;
use Time::HiRes qw(usleep);
local $| = 1;

#if($ARGV[1] eq "INTEL") {
#   if ($#ARGV == 3 ) {
#      print "Make DB Script ERROR: More than two arument is passed to script\n";
#      exit;
#   }
#} else {
#   if ($#ARGV == 4 ) {
#      print "Make DB Script ERROR: More than two arument is passed to script\n";
#      exit;
#   }
#}
########################################################### 
# Reading File name and creating DB file name.
# my $OrgFileName   = $ARGV[0];  # Copied File name in to a varible.
# my @spltname      = split('/', $OrgFileName);
my $numelem       = $#spltname;
my $fileName      = $spltname[$numelem]; 

#variable declaration.
my @arryOfLogId      = 0;
my $moduleId         = 0;
my $fileId           = 0;
my $moduleName       = 0;
my $lineNum          = 0;
my $test             = 0;
my $printString      = 0;
my @varibles         = 0;
my $logLvl           = 0;
my $splArgName       = 0;
my $splArgVal        = 0;
my $tokenID          = 0;
my $hexTokenID       = 0;
my $CStart           = 0;
my $CPattern         = 0;
my $CVarible         = 0;
my $CString          = 0;
my $CEnd             = 0;
my $NewVarible       = 0;
my $delimit          = '%8ld';
#my $isModuleNameFound = 0;
my $greatestTokenID  = 0;
my $DBFILE;
my $tmpfile;
my $tmpfile1;
my $dbentry;

###########################################################
# Declare the subroutines
sub trim($);
sub ltrim($);
sub rtrim($);
sub findDup;

# Perl trim function to remove whitespace from the start and end of the string
sub trim($) {
   my $string = shift;
   $string =~ s/^\s+//;
   $string =~ s/\s+$//;
   return $string;
}

# Left trim function to remove leading whitespace
sub ltrim($) {
   my $string = shift;
   $string =~ s/^\s+//;
   return $string;
}

# Right trim function to remove trailing whitespace
sub rtrim($) {
   my $string = shift;
   $string =~ s/\s+$//;
   return $string;
}

my $prj = $ARGV[0];
my $tmp = $ARGV[0];
my @dbFilesDir = ($ARGV[2]);

# Process the file and generate the log DB for the file
#print "Arguments passed are ", @ARGV, "\n";
#print "Calling process of the file with argument ", $ARGV[1], "\n";
parse_preproc_file($ARGV[1]);

sub parse_preproc_file {
    my $OrgFileName = shift;
    #print "File passed is ", $OrgFileName, "\n";
    chomp($OrgFileName);
    my $generatedb = 0;
    my $dbFileName  = sprintf("%s%s", $OrgFileName,".db");
    my $logNum           = 0;

    my $isModuleIdFound = 0;
    my $isFileIdFound = 0;
    my $isModuleNameFound = 0;
    $logNum = 0;
    $fileId = 0;

    # Opening source.i file and DB file        
    # print "FILE:$OrgFileName\n";
    `cp -f $OrgFileName $OrgFileName.bak`;
    if($tmp eq "INTEL") {
       #print "Removing # for INTEL";
       system("sed -i '/^#/d' $OrgFileName");
    } elsif($tmp eq "XEON"){
       #print "Remove # for XEON";
       system("sed -i '/stdlib\-bsearch.h/s/^#//g' $OrgFileName");
       system("sed -i '/^#/d' $OrgFileName");
       system("sed -i '/stdlib\-bsearch.h/s/^/#/g' $OrgFileName");
    }else {
       #print "Don't Remove # for BRCM";
       #system("sed -i '/^#/d' $OrgFileName");
    }   

    open FILE, "<$OrgFileName" or die $!;
    open my $OUTPFILE, '>', "$OrgFileName.tmp" or die "Can't write new file: $!";
    my $fromFound =0;
   
    while(<FILE>) {  
        if (( $_ =~ /logLev[0-4EHS]/ ) && ( $_ !~  /}[;]*/) && ( $_ !~  /R_LOG_LEVEL/)) { 
            $fromFound = 1;
            $concatStr=$_;
            chomp($concatStr);
            next;
        }
   
        if( ($_ =~ /}[;]*/) && ($fromFound == 1) ) {  
            $concatStr =~ s/(.*)(logLev[E0-4HS]) +(.*)/$1$2$3/g;
            print $OUTPFILE $concatStr;
            print $OUTPFILE $_;
            $fromFound = 0;
            next;
        }

        if ($fromFound == 1) {
            chomp($_);
            $concatStr = $concatStr.$_;
        } else {
            #print $OUTPFILE "/* AARTI4*/";
            $_ =~ s/(.*)(logLev[E0-4HS]) +(.*)/$1$2$3/g;
            print $OUTPFILE $_;
        }
    }
    close(FILE);
    close($OUTPFILE);
    `mv -f $OrgFileName.tmp $OrgFileName`;
   
    open(INFILE, "< $OrgFileName") || die("Make DB Script ERROR: Cannot open file $OrgFileName for parse");
    open $tmpfile, "> $OrgFileName.tmp.i" || die("Data Base ERROR: Cannot Create temporary file");
    open ($dbfile, '>' , "$OrgFileName.tmp.db") || die("Data Base ERROR: Cannot create the file $dbFileName");
    ##########################################################
    # Read each line and create the DB entries
    while (<INFILE>) {
        if (($isModuleIdFound != 1) || ($isFileIdFound != 1) || ($isModuleNameFound !=1)) {
            if (/(\s*.*)RLOG_MODULE_ID(\s*.*)=(\s*.*);/) {
                $moduleId = $3;
                $isModuleIdFound  = 1;
            }

            if (/(\s*.*)RLOG_FILE_ID(\s*.*)=(\s*.*);/) {
                $fileId = $3;
                $isFileIdFound = 1;
                #print "FILE $OrgFileName FILE ID $fileId [$1] [$2] [$3]\n";
            }
            if (/(\s*.*)*RLOG_MODULE_NAME(\s*.*)=(\s*.*);/) {
                $moduleName = $3;
                $moduleName = substr($moduleName,1);
                $moduleName = substr($moduleName, 0, -1);
                $isModuleNameFound = 1;
            }

            print $tmpfile $_;
        } elsif (/(\s*.*)(logLev[E0-4HS]\()(\s*.*\,)(\s*\".*\")(\s*.*)/) {
            $CStart   = $1;
            $CPattern = $2;
            $CVarible = $3;
            $CString  = $4;
            $CEnd     = $5;

            if( $3 =~ m/_LOGID/ ) {
                my    $fmtStr = $CString;
                @variables = split(/,/, $CVarible);
                my $arrSize = $#variables;
                my $line = $variables[$arrSize];
                my $file = $variables[$arrSize-1];

                if ($fileId < 0x3FF) {
                    $tokenID = ($fileId << 16)|(++$logNum); 
                    $hexTokenID = sprintf("0x%x", $tokenID);
                } else { print "SOME ERROR MODID:$moduleId FILEID:$fileId\n";
                    return 1;
                }

                $CVarible =~ s/_LOGID/$hexTokenID/;

                print $tmpfile "$CStart$CPattern$CVarible$CString$CEnd\n";
                print $dbfile "$tokenID;$line;$moduleName;$file;$fmtStr\n";
                $generatedb = 1;
            }
        } else {
            print $tmpfile $_;
        }
        $lineNum = $lineNum + 1;
    }

    ##########################################################################
    #Close all files and create final output file
    close(INFILE);
    close ($tmpfile);
    close ($dbfile);

    #close ($DBFILE);
    #if( $generatedb  eq 1) {
        `cp -f $OrgFileName.tmp.i $OrgFileName`;
        `mv -f $OrgFileName.tmp.db $dbFileName`;
        `dos2unix -q $OrgFileName`;
        `dos2unix -q $dbFileName`;
		`rm -f $OrgFileName.tmp.i`
    #} else {
    #    'rm -f tmp.i';
    #}
}
