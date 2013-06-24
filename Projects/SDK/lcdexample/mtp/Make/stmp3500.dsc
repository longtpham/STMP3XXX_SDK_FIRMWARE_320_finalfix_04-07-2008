/**************************************************************************
**
**  FILE        :  MTP build's STMP3500.dsc
**
**  Historic VERSION     :  00/012/20
**
**  DESCRIPTION :  Default locator software part description;
**                 this is for Sigmatel STMP3500.
**
**      NOTE        :  Values for the size of internal memory spaces are
**                     taken from the CPU file. By default this file
**             assumes that the ROM data tables have not been
**             enabled.
**
**             Defines to change the default memory layout
**             (-em<define> on the locator command line):
**             MEM_Y,
**             MEM_P,
**                     MEM_L : Y/P/L used as default memory (instead of X)
**             NSTK_L: do not put stack in L memory
**                 (only applicable for default X or Y memory)
**             STACKSIZE: set stack size (other than default)
**             HEAPSIZE : set heap size (other than default)
**             NEARXYSIZE : set near X/Y variable area size
**
**************************************************************************/
#define TOTAL_ADDR_SPACE 0x10000

#define INT_Y_ORG   0x0

#define INT_P_RAM   0xe000
#define INT_X_RAM   0x6000
#define INT_Y_RAM   0x6000

#define EXT_X_ORG   INT_X_RAM
#define EXT_Y_ORG   INT_Y_RAM

#define EXT_P_RAM   (TOTAL_ADDR_SPACE-INT_P_RAM)
#define EXT_X_RAM   (TOTAL_ADDR_SPACE-INT_X_RAM)
#define EXT_Y_RAM   (TOTAL_ADDR_SPACE-INT_Y_RAM)

#define INT_P_VECTOR_ORG 0x0000
#define INT_P_VECTOR_SZ  0x0082

#define INT_P_FILESYSTEM_ORG    (INT_P_VECTOR_ORG+INT_P_VECTOR_SZ)
#define INT_P_FILESYSTEM_SZ     (0x34A0)//(0x34BC-0x8)

#define INT_P_MTP_ORG           (INT_P_FILESYSTEM_ORG+INT_P_FILESYSTEM_SZ)
#define INT_P_MTP_SZ            0x1690 //(0x1462)//(0x13a9+140+10) //0x1406//(0x1380) 

#define INT_P_MEDIADD_ORG       (INT_P_MTP_ORG+INT_P_MTP_SZ)
//#define INT_P_MEDIADD_SZ        (0x1092)  //(0x108c)
#define INT_P_MEDIADD_SZ        (0xcd0)//(0x1092-140)  //(0x108c)

#define INT_P_FUNCLET_ORG       (INT_P_MEDIADD_ORG+INT_P_MEDIADD_SZ)
#define INT_P_FUNCLET_SZ        (0x6d3)

#define INT_P_SYS_ORG           (INT_P_FUNCLET_ORG+INT_P_FUNCLET_SZ)
#define INT_P_SYS_SZ            (0xc000 - INT_P_SYS_ORG) //(INT_P_RAM-INT_P_SYS_ORG-0x1f38+3)

// overlay with the Y memory. Only for functions not used after switching to MTP mode
#define INT_P_USBMSC_ORG        (0xc000) //(INT_P_SYS_ORG+INT_P_SYS_SZ)
#define INT_P_USBMSC_SZ         (0x2000) //(0x1f38)

#define INT_X_MTP_BOOTLOAD_ORG  0
#define INT_X_MTP_BOOTLOAD_SZ   0x600 // 0x4A0 3.0.152 ver.     merge from sdk2.6.25 to 3.0beta increased this to 6x00 and locates. Enough for merge from 2.6.39

#define INT_X_MTP_ORG           (INT_X_MTP_BOOTLOAD_ORG+INT_X_MTP_BOOTLOAD_SZ)
#define INT_X_MTP_SZ            0x1D8

#define INT_X_SYS_ORG           (INT_X_MTP_ORG+INT_X_MTP_SZ)
#define INT_X_SYS_SZ            (INT_X_RAM-INT_X_SYS_ORG)

software
{
  load_mod start=Exec;
  layout
  {
    space P
    {
      block P_VectorBlock
      {
        // P-space memory range
        cluster P_vector
        {
          contiguous addr = INT_P_VECTOR_ORG
          {
            section .ptextResetVector;
            section .filler;
          }
        }
      }
      block P_MediaDDBlock
      {
        #include "..\..\..\..\..\CPUFiles\P_MediaDDCluster_MSC.dsc"
      }
      block P_FileSystemBlock
      {
        cluster P_FileSystem_cluster
        {
          amode P_far
          {
            contiguous addr=INT_P_FILESYSTEM_ORG
            {
              label bP_FileSystem_cluster;
              overlay
              {
                contiguous
                {
                  label bFATWrite_P;
                  section .ptextnddcopysectors                MEDIA_DD_COPY;
                  section .ptextnddfindfreeublock             MEDIA_DD_COPY;
                  section .ptextnddstartnewupdate             MEDIA_DD_COPY;
                  section .ptextfsddwrite                     MEDIA_DD_COPY;
                  section .ptextnddkillusectorsmap            MEDIA_DD_COPY;
                  section .ptextnandwritesync                 MEDIA_DD_COPY; // should probably remove line & in fatwritemtp.loc. TOVERIFY3
                  section .ptextnddrecoverbadhblock           MEDIA_DD_COPY;
                  section .ptextnddrecoverbadrblock           MEDIA_DD_COPY;
                  section .ptextnddlingetfreeerasedrblock     MEDIA_DD_COPY;
                  section .ptextnddgetrecycledublock          MEDIA_DD_COPY;
                  section .ptextnddfreshenupdate              MEDIA_DD_COPY;
                  section .ptextnddfggc                       MEDIA_DD_COPY;
                  section .ptextnddaddusector                 MEDIA_DD_COPY;
                  section .ptextnddrecoverbadublock           MEDIA_DD_COPY;
                  section .ptextnddgetfreeerasedublock        MEDIA_DD_COPY;
                  section .ptextnddwritesector                MEDIA_DD_COPY;
                  section .ptextnddkillusector                MEDIA_DD_COPY;
                  section .ptextnddcloseupdate                MEDIA_DD_COPY;
                  section .ptextnandhaltype1writefunctions    MEDIA_DD_COPY;
                  section .ptextnandhaltype2writefunctions    MEDIA_DD_COPY;
                  section .ptextnandhaltype5functions         MEDIA_DD_COPY;
                  section .ptextnandhalcommonwritefunctions   MEDIA_DD_COPY;
                  section .ptextnandra_utils_write            MEDIA_DD_COPY;
                  section .ptextnandhalstartcomputeecc        MEDIA_DD_COPY;
                  section .ptextnandhalsetnandbusy            MEDIA_DD_COPY;
                  section .ptextnandhalcomputeeccgetstatus    MEDIA_DD_COPY;
                  section  NANDHIL_Write_P                    MEDIA_DD_COPY;
                  section .ptextnddgetfreeerasedrblock        MEDIA_DD_COPY;
                  section .ptextnandcalculateeccsync          MEDIA_DD_COPY;
                  section .ptextnandhalmarkblockbadinra       MEDIA_DD_COPY;

                  section .ptextnddkillupdate                 MEDIA_DD_COPY;
//                  section .ptextnddsetsectorlba               MEDIA_DD_COPY; // should probably remove. TOVERIFY3
//                  section .ptextmmchalspiwrite                MEDIA_DD_COPY; //TOVERIFY2 mmc builds may need this again. but comment out for now so we match loc file. TOVERIFY3

                  section .ptextfflush                        MEDIA_DD_COPY;

                  section .ptextflushsector                   MEDIA_DD_COPY;
//                  section .ptextfindnextfreecurrentcluster    MEDIA_DD_COPY; // do we need this? not in fatwritemmtp.loc c file is empty. TOVERIFY3

                  section .ptextupdatefilesize                MEDIA_DD_COPY;
                  section .ptextgetnewcluster                 MEDIA_DD_COPY;
                  section .ptextwritefatentry                 MEDIA_DD_COPY;
                  section .ptextwritesector                   MEDIA_DD_COPY;
                  section .ptextfirstfreeandallocate          MEDIA_DD_COPY;
                  label eFATWrite_P;

                  contiguous{
                    overlay{
                        contiguous{
                            label bWrite_P;
                            section .ptextfwrite                            MEDIA_DD_COPY;
                            label eWrite_P;
                        }
                        contiguous{
                            label bMTPbootinit_P;
                  section .ptexttotalfreecluster              MEDIA_DD_COPY;
                  section .pStmpTotalfreecx                   MEDIA_DD_COPY;
                  section .ptextcomputefreeclusters           MEDIA_DD_COPY;
                            label eMTPbootinit_P;
                        }
                    }
                  }

                }
                contiguous{
                    label b_JanusCommon_P;
                    section  .ptextrc4          JANUS_COPY;
                    section  .ptextblackbox         JANUS_COPY;
                    section  .ptextdrmcbc           JANUS_COPY;
                    section  .ptextdrmcipher            JANUS_COPY;
                    section  .ptextdrmcipher_runtime            JANUS_COPY;
                    section  .pDRM_RC4_Cipher_X         JANUS_COPY;
                    section  .pWMDRMMP_C_STEP           JANUS_COPY;
                    section  .pDRM_DES_Cipher           JANUS_COPY;
                    section  .ptextjanusutils           JANUS_COPY;
                    section  .ptextMtpfunc          JANUS_COPY;
                    section  .ptextJanusfunc            JANUS_COPY;
                    section  .ptextpkcryptofunc         JANUS_COPY;
                    section  .ptextmtp_drmcleanstore            JANUS_COPY;
                    section  .ptextoemimpl          JANUS_COPY;
                    section  .ptextdrmcommon        JANUS_COPY;
                    label e_JanusCommon_P;
                    overlay{
                        contiguous{
                            label b_doplay_P;
                            section  .pDRM_RC4_Cipher          JANUS_COPY;
                            section  .ptextdrmsyncstore        JANUS_COPY;
                            section  .ptextdrmint64            JANUS_COPY;
                            section  .ptextdrmhmac             JANUS_COPY;
                            section  .ptextdrmmetering         JANUS_COPY;
                            section  .ptextdrmhdrparser        JANUS_COPY;
                            section  .ptextoeminfo             JANUS_COPY;
                            section  .pWMDRMdes                JANUS_COPY;
                            section  .ptextdrmoutputlevels     JANUS_COPY;
                            section  .ptextdrmliceval          JANUS_COPY;
                            section  .ptextdevicedevcert       JANUS_COPY;
                            section  .ptextdrmbase64           JANUS_COPY;
                            section  .ptextdrmlicstore         JANUS_COPY;
                            section  .ptextdevcert             JANUS_COPY;
                            section  .ptextdrmexprvariable     JANUS_COPY;
                            section  .ptextdrmexprfunc         JANUS_COPY;
                            section  .ptextdrmmanager          JANUS_COPY;
                            section  .ptextdrmchain            JANUS_COPY;
                            section  .ptextdrmviewrights       JANUS_COPY;
                            section  .ptextdrmutilities        JANUS_COPY;
                            section  .ptextdrmxmlparser        JANUS_COPY;
                            section  .ptextdrmsecstore_ovl     JANUS_COPY;
                            section  .ptextdrmutilities_ovl    JANUS_COPY;
                            section  .ptextdrmcrt              JANUS_COPY;
                            section  .ptextdrmhds              JANUS_COPY;
                            section  .ptextdrmexpreval         JANUS_COPY;
                            section  .ptextdrmlicparser        JANUS_COPY;
                            section  .ptextdrmsecstore         JANUS_COPY;
                            label e_doplay_P;
                        }
                        contiguous{
                            label b_MTP_P;
                            section .ptextmtp_drmmanager            JANUS_COPY;
                            section .ptextmtp_drmgensyncchallenge   JANUS_COPY;
                            section .ptextmtp_drmmgrmeter           JANUS_COPY;
                            section .ptextmtp_drmcbc                JANUS_COPY;
                            section .ptextmtp_drmcipher             JANUS_COPY;
                            section .ptextmtp_drmlicstore           JANUS_COPY;
                            section .ptextmtp_drmmetering           JANUS_COPY;
                            section .ptextmtp_drmsyncstore          JANUS_COPY;
                            section .ptextmtp_drmblackbox           JANUS_COPY;
                            section .ptextmtp_drmutilities          JANUS_COPY;
                            section .ptextmtp_drmbase64             JANUS_COPY;
                            section .ptextmtp_drmsecstore           JANUS_COPY;
                            section .ptextmtp_drmxmlparser          JANUS_COPY;
                            section .ptextmtp_drmxmlbuilder         JANUS_COPY;
                            section .ptextmtp_drmlicacqresp         JANUS_COPY;
//                            section  .ptextmtp_drmhds               JANUS_COPY;
                            label e_MTP_P;
                        }

                        contiguous{   // pkcrypto section
                            label b_Pkcrypto_P;
                            section  .ptextmodsqrt          PK_COPY;
                            section  .ptextmpinit           PK_COPY;
                            section  .ptextkdiv             PK_COPY;
                            section  .ptextlucas            PK_COPY;
                            section  .ptextmodexp           PK_COPY;
                            section  .ptextbigerrno         PK_COPY;
                            section  .ptextkmuladd          PK_COPY;
                            section  .ptextmprand           PK_COPY;
                            section  .pAsmBignum            PK_COPY;
                            section  .ptextbigdefs          PK_COPY;
                            section  .ptextpkcrypto         PK_COPY;
                            section  .ptextkinvert          PK_COPY;
                            section  .ptextmpmul22          PK_COPY;
                            section  .ptextbignum           PK_COPY;
                            section  .ptextmodmulch1        PK_COPY;
                            section  .ptextecurve           PK_COPY;
                            section  .ptextkinitpr          PK_COPY;
                            section  .ptextmodular          PK_COPY;
                            section  .ptextfield            PK_COPY;
                            section  .ptextfield1           PK_COPY;
                            section  .ptextdivide1          PK_COPY;
                            section  .ptextmpaddsubcmp      PK_COPY;
                            section  .ptextmpgcdex          PK_COPY;
                            section  .ptextecexpon          PK_COPY;
                            label e_Pkcrypto_P;
                        }
                        contiguous{
                            section .ptextblackbox_init;
                            section .ptextdevcert_init;
                            section .ptextdrmhds_init;
                            section .ptextdrmmanager_init;
                            section .ptextgendevcert;
                            section .ptextmtp_drmlicparser;
                            section .ptextmtp_secureclockrequest;
                            section .ptextmtp_secureclockresponse;
                            section .ptextsecureclockrequest;
                        }

                    }
                }
              }
                section  .ptextdrmcbc_runtime;
                overlay{
                    contiguous{
                        label b_AES_ENC_P;
                        section  .paes_dec_key      JANUS_COPY;
                        section  .paes_enc_key      JANUS_COPY;
                        section  .paes_enc_blk      JANUS_COPY;
                        section  .ptextencdata      JANUS_COPY;
                        section  .pInitADC          JANUS_COPY;
                        label e_AES_ENC_P;
                    }
                    contiguous{
                        label b_HDS_NONOVL_P;
                        section .ptextoemfileio        JANUS_COPY;
                        section .ptextdrmstkalloc      JANUS_COPY;
                        label e_HDS_NONOVL_P;
                }
              }
            }
            label eP_FileSystem_cluster;
          }
        }
      }
      block P_FuncletBlock
      {
        cluster P_Funclet_cluster
        {
          amode P_far
          {
            contiguous addr = INT_P_FUNCLET_ORG
            {
              label bP_space_funclet;
              contiguous
              {
                overlay
                {
                    #include "..\..\..\..\..\CPUFiles\P_FuncletCluster_MTP.dsc"
                }
              }
              label eP_space_funclet;
            }
          }
        }
      }
      block P_MTPBlock
      {
        #include "..\..\..\..\..\CPUFiles\P_MTPCluster.dsc"
      }
      block P_SysBlock
      {
        cluster P_sys_cluster
        {
          amode P_far
          {
            label bP_sys_cluster;
            contiguous addr = INT_P_SYS_ORG
            {
              overlay
              {
                contiguous
                {
                  section .ptextfatsector             MEDIA_DD_COPY;
                  section .ptextfeof                  MEDIA_DD_COPY;
                  section .ptextfindfirst             MEDIA_DD_COPY;
                  section .ptextfindnext              MEDIA_DD_COPY;
                  section .ptextfindnextcluster       MEDIA_DD_COPY;
                  section .ptextfirstsectorofcluster  MEDIA_DD_COPY;
                  section .ptextfread                 MEDIA_DD_COPY;
                  section .ptextfsddread              MEDIA_DD_COPY;
                  section .ptextfsutilsc              MEDIA_DD_COPY;
                  section .ptextgetchar               MEDIA_DD_COPY;
                  section .ptexthandle                MEDIA_DD_COPY;
                  section .ptexthandleres             MEDIA_DD_COPY;
                  section .ptextputchar               MEDIA_DD_COPY;
                  section .ptextreaddirectoryrecord   MEDIA_DD_COPY;
                  section .ptextreadfat12entry        MEDIA_DD_COPY;
                  section .ptextreadsector            MEDIA_DD_COPY;
 			section .ptextShortDirMatch         MEDIA_DD_COPY;
                  section .ptextStrlength             MEDIA_DD_COPY;
                  section .ptextfseek                 MEDIA_DD_COPY;
                  section .ptextreadrootdirrecord     MEDIA_DD_COPY;
                  section .ptextreadfatentry          MEDIA_DD_COPY;
                  section .ptextfgetc                 MEDIA_DD_COPY;
                  section .ptextftell                 MEDIA_DD_COPY;
                  section .ptextfopen                 MEDIA_DD_COPY;
                  section .ptextfopenw                MEDIA_DD_COPY;
                  section .ptextfputc                 MEDIA_DD_COPY;
                  section .ptextfputs                 MEDIA_DD_COPY;
                  section .ptextSysFat                MEDIA_DD_COPY;
                  section .ptextFsUtils               MEDIA_DD_COPY;
                  section .ptextgetfilesize           MEDIA_DD_COPY;
                  section .ptextdeletetree            MEDIA_DD_COPY;
                  section .ptextDeleteTree            MEDIA_DD_COPY;
                  section .ptextFgetFastHandle        MEDIA_DD_COPY;
                  section .ptextfremovew              MEDIA_DD_COPY;
                  section .ptextfsddinit              MEDIA_DD_COPY;
                  section .ptextfsinit                MEDIA_DD_COPY;
                  section .ptextfsshutdown            MEDIA_DD_COPY;
                  section .ptextfstrcpy               MEDIA_DD_COPY;
                  section .ptextfsunicode             MEDIA_DD_COPY;
                  section .ptextfastopen              MEDIA_DD_COPY;
                  section .ptextfclose                MEDIA_DD_COPY;
                  section .ptextfcreate               MEDIA_DD_COPY;
                  section .ptextfgetfasthandle        MEDIA_DD_COPY;
                  section .ptextfgets                 MEDIA_DD_COPY;
                  section .ptextfilegetattrib         MEDIA_DD_COPY;
                  section .ptextfilegetdate           MEDIA_DD_COPY;
                  section .ptextfilesetattrib         MEDIA_DD_COPY;
                  section .ptextfilesetdate           MEDIA_DD_COPY;
                  section .ptextfilesystempresent     MEDIA_DD_COPY;
                  section .ptextfindfreerecord        MEDIA_DD_COPY;
                  section .ptextfremove               MEDIA_DD_COPY;
                  section .ptextgetshortfilename      MEDIA_DD_COPY;
                  section .ptextgetvolumelabel        MEDIA_DD_COPY;
                  section .ptextisdirectoryempty      MEDIA_DD_COPY;
                  section .ptextisshortnamevalid      MEDIA_DD_COPY;
                  section .ptextlongdirmatch          MEDIA_DD_COPY;
                  section .ptextlongfilename          MEDIA_DD_COPY;
                  section .ptextmatchdirrecord        MEDIA_DD_COPY;
                  section .ptextmkdir                 MEDIA_DD_COPY;
                  section .ptextrmdir                 MEDIA_DD_COPY;
                  section .ptextrmdirw                MEDIA_DD_COPY;
                  section .ptextsearchdirectory       MEDIA_DD_COPY;
                  section .ptextsetcwd                MEDIA_DD_COPY;
                  section .ptextsethandleforsearch    MEDIA_DD_COPY;
                  section .ptextsetshortfilename      MEDIA_DD_COPY;
                  section .ptextsetvolumelabel        MEDIA_DD_COPY;
                  section .ptextCreateDirectoryRecord MEDIA_DD_COPY;
                  section .ptextExtractFileName       MEDIA_DD_COPY;
                  section .ptextExtractPath           MEDIA_DD_COPY;
                  section .ptextFSGetDataDrivePbsLocation     MEDIA_DD_COPY;
                  section .ptextFsunicode             MEDIA_DD_COPY;
                  section .ptextarrangefilename       MEDIA_DD_COPY;
                  section .ptextarrangelongfilename   MEDIA_DD_COPY;
                  section .ptextchangepath            MEDIA_DD_COPY;
                  section .ptextchangetorootdirectory MEDIA_DD_COPY;
                  section .ptextchdir                 MEDIA_DD_COPY;
                  section .ptextcheckspaceinrootdir   MEDIA_DD_COPY;
                  section .ptextcreatedirectory       MEDIA_DD_COPY;
                  section .ptextcreatedirectoryrecord MEDIA_DD_COPY;
                  section .ptextcreateshortdirrecord  MEDIA_DD_COPY;
                  section .ptextextractfilename       MEDIA_DD_COPY;
                  section .ptextextractpath           MEDIA_DD_COPY;
                  section .ptextshortdirmatch         MEDIA_DD_COPY;
                  section .ptextLongDirMatch          MEDIA_DD_COPY;
                  section .ptextSetShortFileName      MEDIA_DD_COPY;
                  section .ptextgetname               MEDIA_DD_COPY;
                  section .ptextreaddevicerecord      MEDIA_DD_COPY;
                  section .ptextGetName               MEDIA_DD_COPY;
                  section .ptextdiscardtrailigperiods MEDIA_DD_COPY;
                  section .ptextdeleterecord          MEDIA_DD_COPY;
                  section .ptextdeletecontent         MEDIA_DD_COPY;

                  section .ptextfindnextsector        MEDIA_DD_COPY;
                  section .ptexttransport             MEDIA_DD_COPY;

                  section .ptextmtprequests           MEDIA_DD_COPY;
                  section .ptextmtp_services          MEDIA_DD_COPY;
//                  section .ptextstobjects             MEDIA_DD_COPY;
                  section .ptextstgetnumobjects       MEDIA_DD_COPY;
                  section .ptextstindexfilesio        MEDIA_DD_COPY;

                }
                contiguous
                {
                  section .ptextusbmscrequests;
                  section .ptextusbmscproject;
                  section .ptextusbmsc_services;
                  section .ptextscsich;
                  section .ptextgetcustomerscsihandler;
                  section .ptextmtp_updater_scsi_commands;
                } /* contiguous */

                // nandmediaallocate not called or resident for mtp or in autoswitch msc but only in updater.sb.
                // Maybe needed here to resolve msc reference but can't be called in this build since not loaded.
                section .ptextnandmediaallocate         MEDIA_DD_COPY;

              }
            }
            contiguous
            {
              /////////////////////////////////////////////
              // all P sections not explicitly defined in this
              //file will end up here in Psys, due to next line
              section selection=w;
              /////////////////////////////////////////////
            }
            label eP_sys_cluster;
          }
        }
      }
      block P_USBMSC_Block
      {
        cluster P_USBMSC_cluster
        {
          amode P_far
          {
            label bP_usbmsc_cluster;
            contiguous addr = INT_P_USBMSC_ORG
            {
              section .ptextnanddatadrivemultiwritedumb;
              section .ptextnanddatadrivemultiwritecache;
            }
            section .ptextoverlay_startup;
            section .ptextkernel_startup;
            section .ptextsed15xxinitstatic;
			section .ptexts6b33b0ainitstatic;
            section .ptextscsi;
            section .ptextusbmsc;
            section .ptextnandmediainit_wrapper; // Mar 1 2007: new section to save P_MediaDDBlock
            section .ptextnandmediaerase_wrapper; // Mar 1 2007: new section to save P_MediaDDBlock
            label eP_usbmsc_cluster;
          }
        }
      }
    }        // end of P_space
    space L
    {
      // L address space
      block L_block
      {
        // L-space memory range
        cluster L_clstr
        {
          attribute w;
          amode L_near
          {
            section selection=w;
          }
          amode L_far
          {
            section selection=w;
          }
        }
      }
    }
    space X
    {
      block XMTPBootLoadBlock
      {
        cluster X_BootLoad_Cluster
        {
          amode X_far
          {
            //
            // There is nothing magical about these sections, just
            // that they are bss and in X.  This section is used
            // by the bootloader and must not have initialized
            // data in it, thus we can overlay them.
            //
            contiguous
            {
              section .xbsssystem;
              section .xbssnandhalcommonfunctions;
              section .xbssnanddatadriveinit;
              section .xbssusb_api;
              section .xbssusb_descriptor.lcl_String0;
//              section .xbssusb_descriptor.lcl_String1;
//              section .xbssusb_descriptor.lcl_String2;
              section .xbssnddgetusectorsmap;
              section .xbssnandhalreadfunctions;
              section .xbssnandhalcommonwritefunctions;
              section .xbssnanddatadrivemultiwritedumb;
            }
          }
        }
      }

      // X address space
      block XMTPBlock
      {
        cluster X_MTP_Cluster
        {
          amode X_near
          {
            // near addressing mode:
            // locate writable sections
            section selection=w;
          }
          amode X_far
          {
            contiguous
            addr = 0x600
            {
              // WARNING!!!  Do not change addr above unless usb20 dd alignment correct.
              // This addr is required because the USB20_Device_Driver
              // must be aligned on a 64 BYTE boundary and also must
              // be on a word aligned address as well. 0x400 & +0x200=0x600 meet this requirement.
              //
              section USB20_Device_Driver;
            }
          }
        }
      }

      block XSYSBlock
      {
        // X-space memory range
        cluster X_clstr
        {
          attribute w;
          amode X_near
          {
            // near addressing mode:
            // locate writable sections
            section selection=w;
          }

          amode X_far
          {
            contiguous
            {
              contiguous
			  {
	              overlay
	              {
	                contiguous
	                {
	                  // Place here all sections required only by MSC
	                  // and can be overlaid with Janus

	                  section .xbssusbmscproject_ovly_mem;

					  // By default scsi sections can be overlay with MTP/Janus
					  section .xdatascsich;

	                }
                    contiguous
                    {
                        label b_DEVINFODATA;
                            section .xdatamtpdeviceinfodata TASK_COPY;
                        label e_DEVINFODATA;
                    }
                    contiguous
                    {
                    // Place here all Janus sections that ca be overlaid
                    // with MSC operation
                            contiguous{
                                overlay{
                                    section .xbsssecstore_mem;
                                    contiguous{
                                        section .xInitADC;
                                        section .xbssjanus_mtp_init;
//                                      section .xdatajanus_mtp_init;
                                    }
                                }
                            }
                            contiguous{
                                section .xbsswmdrmpdmem;
                                overlay{
                                    section .xdevecerttemplate ;
                                    section .xdevcert ;
                                    section .xOverlayBuffer;
                                }
                            }
                            section .xbssdrmliceval;
                            overlay{
                                contiguous{
                                    label b_TABLE_X;
                                    section .xconstdrmconstants     JANUS_COPY;
                                    label e_TABLE_X;
                                }
                                contiguous{
                                    label b_TABLE1_X;
                                    section .xDES_Key           JANUS_COPY;
                                    section .xDES_Key_Setup     JANUS_COPY;
                                    label e_TABLE1_X;
                                }
                            }
                            section .xbssdrmchain;
                            label   b_JANUS_CONST;
                            section .xconstbase64table      JANUS_COPY;
                            section .xconstoeminfo          JANUS_COPY;
                            section .xconstdrmmd5           JANUS_COPY;
                            section .xdatabase64table       JANUS_COPY;
                            section .xdatadrmlicparser      JANUS_COPY;
                            section .xdatamtp_drmmgrmeter   JANUS_COPY;
                            section .xconstmtp_drmconstants JANUS_COPY;
                            section .xdatamtp_drmutilities  JANUS_COPY;
                            label   e_JANUS_CONST;
                    }
	              }
			  }

              contiguous
			  {
	              overlay
	              {
	                contiguous
	                {
	                  // Place here all sections required only by MSC
	                  // and can be overlaid with Janus

	                  section .xbssusbmtp_cmdoutbuf_ovly_mem;

	                }

                    contiguous
                    {
	                // Place here all Janus sections that ca be overlaid
	                // with Device1CommandOutBuffer used for Bulk OUT Transfers
                            section .xbssdrmsyncstore;
                            overlay{
                                section .xbssdrmexprvariable;
                                section .xbssblackbox;
                                section .xbsspseudo;
                            }
                            section .xbssdevcert;
                            overlay{
                                contiguous{
                                    section .xbssencdata;
                                    section .xbssdecryptdata;
                                }
                                contiguous{
                                    section .xbssdrmoutputlevels;
                                    section .xbssdrmxmlparser;
                                    section .xbssdrmutilities;
                                    section .xbssdrmlicparser;
                                    section .xbssdevicedevcert;
                                }
                            }
                            section .xbssdrmviewrights;
                            section .xbssdrmmanager;
                            section .xbssdrmexpreval;
                            section .xbssdrmhdrparser;
                            section .xbssdevcert_init;
                            section .xbssdrmcrt;
                            section .xbssdrmexprfunc;
                            section .xbssdrmmanager_init;
                            section .xbsssecstore_memovl;
                            section .xbssmtp_drmxmlbuilder;
                            section .xbssmtp_drmlicstore;
                            section .xbssmtp_drmlicacqresp;
                            section .xbssmtp_drmhdrparser;
                            section .xbssmtp_drmmanager;
                            section .xbssmtp_drmgensyncchallenge;
                            section .xbssmtp_drmxmlparser;
                            section .xbssmtp_drmmgrmeter;
                            section .xbssmtp_drmutilities;
                            section .xbssmtp_ram;


                    }
                  }
              }

              section selection=w;

              //section .xbssusb_descriptor.lcl_String0; // this 1 fits in mtp X_BootLoad_Cluster not here
              section .xbssusb_descriptor.lcl_String1;
              section .xbssusb_descriptor.lcl_String2;
            }

            reserved addr=0x5fff length=0x1;
          }        // end of amode X_far

          amode X_io
          {
            // io addressing mode:
            // locate writable sections
            section selection=w;
          }
        }
      }

    } /* space X */

    space Y
    {
      // Y address space
      block Y_block
      {
        // Y-space memory range
        cluster Y_clstr
        {
          attribute w;
          section GLOBAL_CONSTANTS_Y addr=0;
          amode Y_near
          {
            section selection=w;
          }

          amode Y_far
          {
            contiguous
            {
                  section .ydatamain;
	          }
	      }

          amode Y_far
          {
            contiguous
            {
              //contiguous
              //{
              //    section .yovlusb20dd;
              //}
              contiguous
			  {
	              overlay
	              {
	                contiguous
	                {
	                  // Place here all sections required only by MSC
	                  // and can be overlaid with Janus

                      section .ybssusbmscproject_ovly_mem;
                      section .ydatausbmsc;

					  // SCSI sections are not by default needed in MTP
					  section .ybssscsi.g_DeviceInsertionQueue;
					  section .yovlscsi;
					  section .ydatascsich;
					  section .ybssscsich;
                      section .yovlmtp_updater_scsi_commands;
                      section .ydatamtp_updater_scsi_commands;
                      section .ybssmtp_updater_scsi_commands;
					  section .ydatascsi;
					  section .ybssscsi;
	                }

                    contiguous
                    {
	                // Place here all Janus sections that ca be overlaid
	                // with MSC operation
                      section .yovljanus_mtp_init;
                      section .ybssjanusdefis;
                      section .ybsskeydefis;
                       overlay{
                            section .ybssdrmsecstore;
                            section .ybssjanusutils;
                        }
                    }
	              }
	              contiguous
	              {
                      section .yovldisplaydrv;
	              }
			  }

              contiguous
              {
                // Put rest of sections not referenced in here.
                section selection=w;
                section .yconststdeleteobject;
              }

			  contiguous
			  {
			  	overlay
			  	{
			  		contiguous
			  		{
	                     section .ybssnanddatadrivemultiwritedumb attr=-bf;
	                     section .ybssnanddatadrivemultiwritecache attr=-bf;
	                }

			  		overlay
					{
							section .yovlmtpjanussendwmdrmpdcommand;
							section .yovlmtpjanuspropertyhandler;
							section .yovlmtpjanussendwmdrmpdrequest;
							section .yovlmtpjanusgetsynclist;
							section .yovlmtpjanussetmeterresponse;
							section .yovlmtpjanusgetlicensestate;
							section .yovlmtpjanussetsecuretimeresponse;
							section .yovlmtpjanussetlicenseresponse;
							section .yovlmtpjanusgetmeterchallenge;
							section .yovlmtpjanusgetsecuretimechallenge;
							section .yovlmtpjanussendmeterchallengequery;
							section .yovlmtpgetstorageinfo;
							section .yovlmtpgetobjectinfo;
							section .yovlmtpgetdeviceinfo;
							section .yovlmtpsendobjectinfo;
					}
				} 	// end of overlay
			  }

            }       // end of contiguous
          }        // end of amode Y_far

          amode Y_io
          {
            section selection=w;
          }
        }
        cluster Y_janus
        {
            amode Y_far
            {
                contiguous{
                    section .ydataaesdectables         JANUS_COPY;
                    overlay{
                        section .ydataaeskeytables     JANUS_COPY;
                        section .ydataaesenctables     JANUS_COPY;
                        contiguous{
                            label b_CONST_JANUS_Y;
                            section .yconstststoreenum              JANUS_COPY;
                            section .yconststreadobjectdata         JANUS_COPY;
                            section .yconststwriteobjectdata        JANUS_COPY;
                            section .yconststindexfilesio           JANUS_COPY;
                            section .yconstptpstring                JANUS_COPY;
                            section .yconstmtp_mp3metadata          JANUS_COPY;
                            section .yconstmtp_vbr                  JANUS_COPY;
                            section .yconstmtprequests              JANUS_COPY;
                            section .yconstoemfileio                JANUS_COPY;
                            section .yconstdrmconstants             JANUS_COPY;
                            section .yconstdrmlicstore              JANUS_COPY;
                            section .yconstdrmsecstore              JANUS_COPY;
                            section .yconstdrmhds                   JANUS_COPY;
                            section .yconstdrmhds_impl              JANUS_COPY;
                            section .yconstdrmoutputlevels          JANUS_COPY;
                            section .yconstdrmsyncstore             JANUS_COPY;
                            section .yconstbase64table              JANUS_COPY;
                            section .yconstsecureclockrequest       JANUS_COPY;
                            section .yconstfilegetattrib            JANUS_COPY;
                            section .yconstfilesetdate              JANUS_COPY;
                            section .yconstmtp_drmmgrmeter          JANUS_COPY;
                            section .yconstmtp_drmconstants         JANUS_COPY;
                            section .yconstmtp_drmxmlbuilder        JANUS_COPY;
                            section .yconstpkcrypto JANUS_COPY;
                            section .yconstkinitpr  JANUS_COPY;
                            section .yconstmodexp   JANUS_COPY;
                            label e_CONST_JANUS_Y;
                         }
                    }

                    section .ybssdrmcrt;

                    label b_DATA_JANUS_Y;
                    section .ydatadrmhdrparser  JANUS_COPY;
                    section .ydatadrmsyncstore  JANUS_COPY;
                    section .ydatadrmmanager_init   JANUS_COPY;
                    section .ydatadrmutilities  JANUS_COPY;
                    section .ydataoemfileio JANUS_COPY;
                    section .ydatajanusutils    JANUS_COPY;
                    //section .ydatajanusdefis    JANUS_COPY;
                    section .ydatadrmexprvariable   JANUS_COPY;
                    section .ydatadrmexprfunc   JANUS_COPY;
                    section .ydatamtp_drmlicacqresp JANUS_COPY;
                    section .ydatamtp_drmutilities  JANUS_COPY;
                    section .ydatamtp_drmmgrmeter   JANUS_COPY;
                    section .ydatadrmcbc_runtime    JANUS_COPY;
                    section .ydatapkcrypto  JANUS_COPY;
                    label e_DATA_JANUS_Y;

                    section .ybssdrmxmlparser;
                    section .ybssmtp_drmmgrmeter;
                    section .ybssdrmviewrights;
                    section .ybssdrmexpreval;
                    section .ybssjanus_mtp_init;
                    section .ybssdatabase;
                    section .ybsspkcrypto;
                    section .ybssmtp_drmlicacqresp;
                    section .ybssdevcert;
                    section .ybssdecryptdata;
                    section .ybssdrmchain;

                    overlay
                    {
                      section .ybsswmdrmpdmem;
                      section .ybssmtpformatstore; // overlay a large sector size buffer (683 words) This & wmdrmpdmem was overlaid in sdk3.05
                      // sdk3.100 warn; mtp drm in 1st bss overlay here may call fopen which calls deletecontent so can't overlay .ybssdeletecontent here!
                    }

                    section .ybssoemfileio;
                    section .ybssdrmhds_impl;
                    section .ybssdrmlicstore;
                    section .ybssmtp_drmutilities;
                    section .ybssdrmlicparser;


                    section .DRM_DES_Cipher;
                    section DRM_DES_Cipher;
                    section .yAesMem;
                    section .yovlwmdrmsupport;
                    section .yovlJanus;
                    section .yovljanusdrmstubs;
                    section .yovlblackbox_runtime;
                    section .yovldrmmanager_runtime;
                    section .yovlrc4;
                    section .yovlgetinfo;
                    section .yovldrmcipher_runtime;
                    overlay{
                        section .yovldrmcbc_runtime;
                        section .yovloemtime;
                        section .yovlprivate_key;
                    }
                    section .ybssmtp_metadata; // size 200 words aug 12 2005. moved to here in sdk3.1 so scratch buf can grow from 512w to 683w
                }
           }
        }
      }
      // space for overlay copies Y_settings and P overlays
      block YExtBlock
      {
        cluster Y_Copy
        {
          //VERY IMPORTANT!  DO NOT REMOVE THIS attribute s line!!!!
          attribute s;//don't locate anything here automatically.
          //VERY IMPORTANT!  DO NOT REMOVE THIS attribute s line!!!!

          // this cluster doesn't have a "runtime" area
          amode Y_far
          {   //!!!! this label must be inside amode of 1st cluster in ExtY
            label eY_phys;
            contiguous
            {
                // SYSFUN_SysSettings MUST BE LAST!!
                section SYSFUN_SysSettings_Commands_Y;
            }
          }

        }

        #include "..\..\..\..\..\CPUFiles\ycopy.dsc"

      } // YExt block

    }
  }
}


// include cpu part and memory part
cpu ..\..\..\..\..\cpufiles\mtp.cpu
memory ..\..\..\..\..\cpufiles\mtp.mem
