
namespace BW
{
  /**
   * Enumeration of the Battle.net commands. See http://www.bnetdocs.org/
   */
  namespace BNET
  {
    enum Enum
    {
      SID_NULL                           = 0x00,
      SID_UNKNOWN_0X01                   = 0x01,
      SID_STOPADV                        = 0x02,
      SID_UNKNOWN_0X03                   = 0x03,
      SID_SERVERLIST                     = 0x04,
      SID_CLIENTID                       = 0x05,
      SID_STARTVERSIONING                = 0x06,
      SID_REPORTVERSION                  = 0x07,
      SID_STARTADVEX                     = 0x08,
      SID_GETADVLISTEXRESEARCH           = 0x09,
      SID_ENTERCHAT                      = 0x0A,
      SID_GETCHANNELLIST                 = 0x0B,
      SID_JOINCHANNEL                    = 0x0C,
      SID_UNKNOWN_0X0D                   = 0x0D,
      SID_CHATCOMMAND                    = 0x0E,
      SID_CHATEVENT                      = 0x0F,
      SID_LEAVECHAT                      = 0x10,
      SID_UNKNOWN_0X11                   = 0x11,
      SID_LOCALEINFO                     = 0x12,
      SID_FLOODDETECTED                  = 0x13,
      SID_UDPPINGRESPONSE                = 0x14,
      SID_CHECKAD                        = 0x15,
      SID_CLICKAD                        = 0x16,
      SID_UNKNOWN_0X17                   = 0x17,
      SID_REGISTRYDEFUNCT                = 0x18,
      SID_MESSAGEBOX                     = 0x19,
      SID_STARTADVEX2DEFUNCT             = 0x1A,
      SID_GAMEDATAADDRESSDEFUNCT         = 0x1B,
      SID_STARTADVEX3RESEARCH            = 0x1C,
      SID_LOGONCHALLENGEEX               = 0x1D,
      SID_CLIENTID2                      = 0x1E,
      SID_LEAVEGAME                      = 0x1F,
      SID_UNKNOWN_0X20                   = 0x20,
      SID_DISPLAYAD                      = 0x21,
      SID_NOTIFYJOIN                     = 0x22,
      SID_SETCOOKIE                      = 0x23,
      SID_GETCOOKIE                      = 0x24,
      SID_PING                           = 0x25,
      SID_READUSERDATARESEARCH           = 0x26,
      SID_WRITEUSERDATA                  = 0x27,
      SID_LOGONCHALLENGE                 = 0x28,
      SID_LOGONRESPONSE                  = 0x29,
      SID_CREATEACCOUNT                  = 0x2A,
      SID_SYSTEMINFO                     = 0x2B,
      SID_GAMERESULT                     = 0x2C,
      SID_GETICONDATA                    = 0x2D,
      SID_GETLADDERDATA                  = 0x2E,
      SID_FINDLADDERUSER                 = 0x2F,
      SID_CDKEY                          = 0x30,
      SID_CHANGEPASSWORD                 = 0x31,
      SID_CHECKDATAFILEDEFUNCT           = 0x32,
      SID_GETFILETIME                    = 0x33,
      SID_QUERYREALMSDEFUNCT             = 0x34,
      SID_PROFILE                        = 0x35,
      SID_CDKEY2                         = 0x36,
      SID_UNKNOWN_0X37                   = 0x37,
      SID_UNKNOWN_0X38                   = 0x38,
      SID_UNKNOWN_0X39                   = 0x39,
      SID_LOGONRESPONSE2                 = 0x3A,
      SID_UNKNOWN_0X3B                   = 0x3B,
      SID_CHECKDATAFILE2                 = 0x3C,
      SID_CREATEACCOUNT2                 = 0x3D,
      SID_LOGONREALMEX                   = 0x3E,
      SID_STARTVERSIONING2               = 0x3F,
      SID_QUERYREALMS2                   = 0x40,
      SID_QUERYADURL                     = 0x41,
      SID_UNKNOWN_0X42                   = 0x42,
      SID_UNKNOWN_0X43                   = 0x43,
      SID_WARCRAFTGENERALRESEARCH        = 0x44,
      SID_NETGAMEPORT                    = 0x45,
      SID_NEWS_INFO                      = 0x46,
      SID_UNKNOWN_0X47                   = 0x47,
      SID_UNKNOWN_0X48                   = 0x48,
      SID_UNKNOWN_0X49                   = 0x49,
      SID_OPTIONALWORK                   = 0x4A,
      SID_EXTRAWORK                      = 0x4B,
      SID_REQUIREDWORK                   = 0x4C,
      SID_UNKNOWN_0X4D                   = 0x4D,
      SID_TOURNAMENT                     = 0x4E,
      SID_UNKNOWN_0X4F                   = 0x4F,
      SID_AUTH_INFO                      = 0x50,
      SID_AUTH_CHECK                     = 0x51,
      SID_AUTH_ACCOUNTCREATE             = 0x52,
      SID_AUTH_ACCOUNTLOGON              = 0x53,
      SID_AUTH_ACCOUNTLOGONPROOF         = 0x54,
      SID_AUTH_ACCOUNTCHANGE             = 0x55,
      SID_AUTH_ACCOUNTCHANGEPROOF        = 0x56,
      SID_AUTH_ACCOUNTUPGRADEDEFUNCT     = 0x57,
      SID_AUTH_ACCOUNTUPGRADEPROOFDEFUNCT= 0x58,
      SID_SETEMAIL                       = 0x59,
      SID_RESETPASSWORD                  = 0x5A,
      SID_CHANGEEMAIL                    = 0x5B,
      SID_SWITCHPRODUCT                  = 0x5C,
      SID_UNKNOWN_0X5D                   = 0x5D,
      SID_WARDEN                         = 0x5E,
      SID_UNKNOWN_0X5F                   = 0x5F,
      SID_GAMEPLAYERSEARCH               = 0x60,
      SID_UNKNOWN_0X61                   = 0x61,
      SID_UNKNOWN_0X62                   = 0x62,
      SID_UNKNOWN_0X63                   = 0x63,
      SID_UNKNOWN_0X64                   = 0x64,
      SID_FRIENDSLIST                    = 0x65,
      SID_FRIENDSUPDATE                  = 0x66,
      SID_FRIENDSADD                     = 0x67,
      SID_FRIENDSREMOVE                  = 0x68,
      SID_FRIENDSPOSITION                = 0x69,
      SID_UNKNOWN_0X6A                   = 0x6A,
      SID_UNKNOWN_0X6B                   = 0x6B,
      SID_UNKNOWN_0X6C                   = 0x6C,
      SID_UNKNOWN_0X6D                   = 0x6D,
      SID_UNKNOWN_0X6E                   = 0x6E,
      SID_UNKNOWN_0X6F                   = 0x6F,
      SID_CLANFINDCANDIDATES             = 0x70,
      SID_CLANINVITEMULTIPLE             = 0x71,
      SID_CLANCREATIONINVITATION         = 0x72,
      SID_CLANDISBAND                    = 0x73,
      SID_CLANMAKECHIEFTAIN              = 0x74,
      SID_CLANINFO                       = 0x75,
      SID_CLANQUITNOTIFY                 = 0x76,
      SID_CLANINVITATION                 = 0x77,
      SID_CLANREMOVEMEMBER               = 0x78,
      SID_CLANINVITATIONRESPONSE         = 0x79,
      SID_CLANRANKCHANGE                 = 0x7A,
      SID_CLANSETMOTD                    = 0x7B,
      SID_CLANMOTD                       = 0x7C,
      SID_CLANMEMBERLIST                 = 0x7D,
      SID_CLANMEMBERREMOVED              = 0x7E,
      SID_CLANMEMBERSTATUSCHANGE         = 0x7F,
      SID_UNKNOWN_0X80                   = 0x80,
      SID_CLANMEMBERRANKCHANGE           = 0x81,
      SID_CLANMEMBERINFORMATION          = 0x82
    };
  };
};
