#pragma once

#include <chrono>
#include <cstdarg>
#include <functional>
#include <memory>
#include <string>

using namespace std::literals::chrono_literals;

/**
 * @brief An open-source C++ terminal manipulation library made to develop cross-platform apps to for Windows and Linux. It uses an object-oriented philosophy to handle terminal
 * attributes, colors, effects, arguments and event readings. It primarily uses UTF-8 encoding, but has some exceptions that use UTF-16 encoding to fit within the Windows runtime
 * ecosystem.
 */
namespace tmk
{
    /**
     * @brief Contains the POSIX exit codes.
     */
    enum class ExitCode
    {
        /**
         * @brief Success.
         */
        Success = 0,
        /**
         * @brief Operation not permitted (EPERM).
         */
        OperationNotPermittedEPERM = 1,
        /**
         * @brief No such file or directory (ENOENT).
         */
        NoSuchFileOrDirectoryENOENT = 2,
        /**
         * @brief No such process (ESRCH).
         */
        NoSuchProcessESRCH = 3,
        /**
         * @brief Interrupted system call (EINTR).
         */
        InterruptedSystemCallEINTR = 4,
        /**
         * @brief InputStream/output error (EIO).
         */
        InputOutputErrorEIO = 5,
        /**
         * @brief No such device or address (ENXIO).
         */
        NoSuchDeviceOrAddressENXIO = 6,
        /**
         * @brief Argument list too long (E2BIG).
         */
        ArgumentListTooLongE2BIG = 7,
        /**
         * @brief Exec format error (ENOEXEC).
         */
        ExecFormatErrorENOEXEC = 8,
        /**
         * @brief Bad file descriptor (EBADF).
         */
        BadFileDescriptorEBADF = 9,
        /**
         * @brief No child processes (ECHILD).
         */
        NoChildProcessesECHILD = 10,
        /**
         * @brief Resource temporarily unavailable (EAGAIN).
         */
        ResourceTemporarilyUnavailableEAGAIN = 11,
        /**
         * @brief Cannot allocate memory (ENOMEM).
         */
        CannotAllocateMemoryENOMEM = 12,
        /**
         * @brief Permission denied (EACCES).
         */
        PermissionDeniedEACCES = 13,
        /**
         * @brief Bad address (EFAULT).
         */
        BadAddressEFAULT = 14,
        /**
         * @brief Block device required (ENOTBLK).
         */
        BlockDeviceRequiredENOTBLK = 15,
        /**
         * @brief Device or resource busy (EBUSY).
         */
        DeviceOrResourceBusyEBUSY = 16,
        /**
         * @brief File exists (EEXIST).
         */
        FileExistsEEXIST = 17,
        /**
         * @brief Invalid cross-device link (EXDEV).
         */
        InvalidCrossDeviceLinkEXDEV = 18,
        /**
         * @brief No such device (ENODEV).
         */
        NoSuchDeviceENODEV = 19,
        /**
         * @brief Not a directory (ENOTDIR).
         */
        NotADirectoryENOTDIR = 20,
        /**
         * @brief Is a directory (EISDIR).
         */
        IsADirectoryEISDIR = 21,
        /**
         * @brief Invalid argument (EINVAL).
         */
        InvalidArgumentEINVAL = 22,
        /**
         * @brief Too many open files in system (ENFILE).
         */
        TooManyOpenFilesInSystemENFILE = 23,
        /**
         * @brief Too many open files (EMFILE).
         */
        TooManyOpenFilesEMFILE = 24,
        /**
         * @brief Inappropriate ioctl for device (ENOTTY).
         */
        InappropriateIoctlForDeviceENOTTY = 25,
        /**
         * @brief Text file busy (ETXTBSY).
         */
        TextFileBusyETXTBSY = 26,
        /**
         * @brief File too large (EFBIG).
         */
        FileTooLargeEFBIG = 27,
        /**
         * @brief No space left on device (ENOSPC).
         */
        NoSpaceLeftOnDeviceENOSPC = 28,
        /**
         * @brief Illegal seek (ESPIPE).
         */
        IllegalSeekESPIPE = 29,
        /**
         * @brief Read-only file system (EROFS).
         */
        ReadOnlyFileSystemEROFS = 30,
        /**
         * @brief Too many links (EMLINK).
         */
        TooManyLinksEMLINK = 31,
        /**
         * @brief Broken pipe (EPIPE).
         */
        BrokenPipeEPIPE = 32,
        /**
         * @brief Numerical argument out of domain (EDOM).
         */
        NumericalArgumentOutOfDomainEDOM = 33,
        /**
         * @brief Numerical result out of range (ERANGE).
         */
        NumericalResultOutOfRangeERANGE = 34,
        /**
         * @brief Resource deadlock avoided (EDEADLK).
         */
        ResourceDeadlockAvoidedEDEADLK = 35,
        /**
         * @brief File name too long (ENAMETOOLONG).
         */
        FileNameTooLongENAMETOOLONG = 36,
        /**
         * @brief No locks available (ENOLCK).
         */
        NoLocksAvailableENOLCK = 37,
        /**
         * @brief Function not implemented (ENOSYS).
         */
        FunctionNotImplementedENOSYS = 38,
        /**
         * @brief Directory not empty (ENOTEMPTY).
         */
        DirectoryNotEmptyENOTEMPTY = 39,
        /**
         * @brief Too many levels of symbolic links (ELOOP).
         */
        TooManyLevelsOfSymbolicLinksELOOP = 40,
        /**
         * @brief Resource temporarily unavailable (EWOULDBLOCK).
         */
        ResourceTemporarilyUnavailableEWOULDBLOCK = 11,
        /**
         * @brief No message of desired type (ENOMSG).
         */
        NoMessageOfDesiredTypeENOMSG = 42,
        /**
         * @brief Identifier removed (EIDRM).
         */
        IdentifierRemovedEIDRM = 43,
        /**
         * @brief Channel number out of range (ECHRNG).
         */
        ChannelNumberOutOfRangeECHRNG = 44,
        /**
         * @brief Level 2 not synchronized (EL2NSYNC).
         */
        Level2NotSynchronizedEL2NSYNC = 45,
        /**
         * @brief Level 3 halted (EL3HLT).
         */
        Level3HaltedEL3HLT = 46,
        /**
         * @brief Level 3 reset (EL3RST).
         */
        Level3ResetEL3RST = 47,
        /**
         * @brief Link number out of range (ELNRNG).
         */
        LinkNumberOutOfRangeELNRNG = 48,
        /**
         * @brief Protocol driver not attached (EUNATCH).
         */
        ProtocolDriverNotAttachedEUNATCH = 49,
        /**
         * @brief No CSI structure available (ENOCSI).
         */
        NoCSIStructureAvailableENOCSI = 50,
        /**
         * @brief Level 2 halted (EL2HLT).
         */
        Level2HaltedEL2HLT = 51,
        /**
         * @brief Invalid exchange (EBADE).
         */
        InvalidExchangeEBADE = 52,
        /**
         * @brief Invalid request descriptor (EBADR).
         */
        InvalidRequestDescriptorEBADR = 53,
        /**
         * @brief Exchange full (EXFULL).
         */
        ExchangeFullEXFULL = 54,
        /**
         * @brief No anode (ENOANO).
         */
        NoAnodeENOANO = 55,
        /**
         * @brief Invalid request code (EBADRQC).
         */
        InvalidRequestCodeEBADRQC = 56,
        /**
         * @brief Invalid slot (EBADSLT).
         */
        InvalidSlotEBADSLT = 57,
        /**
         * @brief Resource deadlock avoided (EDEADLOCK).
         */
        ResourceDeadlockAvoidedEDEADLOCK = 35,
        /**
         * @brief Bad font file format (EBFONT).
         */
        BadFontFileFormatEBFONT = 59,
        /**
         * @brief Device not a stream (ENOSTR).
         */
        DeviceNotAStreamENOSTR = 60,
        /**
         * @brief No data available (ENODATA).
         */
        NoDataAvailableENODATA = 61,
        /**
         * @brief Timer expired (ETIME).
         */
        TimerExpiredETIME = 62,
        /**
         * @brief Out of streams resources (ENOSR).
         */
        OutOfStreamsResourcesENOSR = 63,
        /**
         * @brief Machine is not on the network (ENONET).
         */
        MachineIsNotOnTheNetworkENONET = 64,
        /**
         * @brief Package not installed (ENOPKG).
         */
        PackageNotInstalledENOPKG = 65,
        /**
         * @brief Object is remote (EREMOTE).
         */
        ObjectIsRemoteEREMOTE = 66,
        /**
         * @brief Link has been severed (ENOLINK).
         */
        LinkHasBeenSeveredENOLINK = 67,
        /**
         * @brief Advertise error (EADV).
         */
        AdvertiseErrorEADV = 68,
        /**
         * @brief Srmount error (ESRMNT).
         */
        SrmountErrorESRMNT = 69,
        /**
         * @brief Communication error on send (ECOMM).
         */
        CommunicationErrorOnSendECOMM = 70,
        /**
         * @brief Protocol error (EPROTO).
         */
        ProtocolErrorEPROTO = 71,
        /**
         * @brief Multihop attempted (EMULTIHOP).
         */
        MultihopAttemptedEMULTIHOP = 72,
        /**
         * @brief RFS specific error (EDOTDOT).
         */
        RFSSpecificErrorEDOTDOT = 73,
        /**
         * @brief Bad message (EBADMSG).
         */
        BadMessageEBADMSG = 74,
        /**
         * @brief Value too large for defined data type (EOVERFLOW).
         */
        ValueTooLargeForDefinedDataTypeEOVERFLOW = 75,
        /**
         * @brief Name not unique on network (ENOTUNIQ).
         */
        NameNotUniqueOnNetworkENOTUNIQ = 76,
        /**
         * @brief File descriptor in bad state (EBADFD).
         */
        FileDescriptorInBadStateEBADFD = 77,
        /**
         * @brief Remote address changed (EREMCHG).
         */
        RemoteAddressChangedEREMCHG = 78,
        /**
         * @brief Can not access a needed shared library (ELIBACC).
         */
        CanNotAccessANeededSharedLibraryELIBACC = 79,
        /**
         * @brief Accessing a corrupted shared library (ELIBBAD).
         */
        AccessingACorruptedSharedLibraryELIBBAD = 80,
        /**
         * @brief .lib section in a.out corrupted (ELIBSCN).
         */
        LibSectionInAOutCorruptedELIBSCN = 81,
        /**
         * @brief Attempting to link in too many shared libraries (ELIBMAX).
         */
        AttemptingToLinkInTooManySharedLibrariesELIBMAX = 82,
        /**
         * @brief Cannot exec a shared library directly (ELIBEXEC).
         */
        CannotExecASharedLibraryDirectlyELIBEXEC = 83,
        /**
         * @brief Invalid or incomplete multibyte or wide character (EILSEQ).
         */
        InvalidOrIncompleteMultibyteOrWideCharacterEILSEQ = 84,
        /**
         * @brief Interrupted system call should be restarted (ERESTART).
         */
        InterruptedSystemCallShouldBeRestartedERESTART = 85,
        /**
         * @brief Streams pipe error (ESTRPIPE).
         */
        StreamsPipeErrorESTRPIPE = 86,
        /**
         * @brief Too many users (EUSERS).
         */
        TooManyUsersEUSERS = 87,
        /**
         * @brief Socket operation on non-socket (ENOTSOCK).
         */
        SocketOperationOnNonSocketENOTSOCK = 88,
        /**
         * @brief Destination address required (EDESTADDRREQ).
         */
        DestinationAddressRequiredEDESTADDRREQ = 89,
        /**
         * @brief Message too long (EMSGSIZE).
         */
        MessageTooLongEMSGSIZE = 90,
        /**
         * @brief Protocol wrong type for socket (EPROTOTYPE).
         */
        ProtocolWrongTypeForSocketEPROTOTYPE = 91,
        /**
         * @brief Protocol not available (ENOPROTOOPT).
         */
        ProtocolNotAvailableENOPROTOOPT = 92,
        /**
         * @brief Protocol not supported (EPROTONOSUPPORT).
         */
        ProtocolNotSupportedEPROTONOSUPPORT = 93,
        /**
         * @brief Socket type not supported (ESOCKTNOSUPPORT).
         */
        SocketTypeNotSupportedESOCKTNOSUPPORT = 94,
        /**
         * @brief Operation not supported (EOPNOTSUPP).
         */
        OperationNotSupportedEOPNOTSUPP = 95,
        /**
         * @brief Protocol family not supported (EPFNOSUPPORT).
         */
        ProtocolFamilyNotSupportedEPFNOSUPPORT = 96,
        /**
         * @brief Address family not supported by protocol (EAFNOSUPPORT).
         */
        AddressFamilyNotSupportedByProtocolEAFNOSUPPORT = 97,
        /**
         * @brief Address already in use (EADDRINUSE).
         */
        AddressAlreadyInUseEADDRINUSE = 98,
        /**
         * @brief Cannot assign requested address (EADDRNOTAVAIL).
         */
        CannotAssignRequestedAddressEADDRNOTAVAIL = 99,
        /**
         * @brief Network is down (ENETDOWN).
         */
        NetworkIsDownENETDOWN = 100,
        /**
         * @brief Network is unreachable (ENETUNREACH).
         */
        NetworkIsUnreachableENETUNREACH = 101,
        /**
         * @brief Network dropped connection on reset (ENETRESET).
         */
        NetworkDroppedConnectionOnResetENETRESET = 102,
        /**
         * @brief Software caused connection abort (ECONNABORTED).
         */
        SoftwareCausedConnectionAbortECONNABORTED = 103,
        /**
         * @brief Connection reset by peer (ECONNRESET).
         */
        ConnectionResetByPeerECONNRESET = 104,
        /**
         * @brief No buffer space available (ENOBUFS).
         */
        NoBufferSpaceAvailableENOBUFS = 105,
        /**
         * @brief Transport endpoint is already connected (EISCONN).
         */
        TransportEndpointIsAlreadyConnectedEISCONN = 106,
        /**
         * @brief Transport endpoint is not connected (ENOTCONN).
         */
        TransportEndpointIsNotConnectedENOTCONN = 107,
        /**
         * @brief Cannot send after transport endpoint shutdown (ESHUTDOWN).
         */
        CannotSendAfterTransportEndpointShutdownESHUTDOWN = 108,
        /**
         * @brief Too many references: cannot splice (ETOOMANYREFS).
         */
        TooManyReferencesCannotSpliceETOOMANYREFS = 109,
        /**
         * @brief Connection timed out (ETIMEDOUT).
         */
        ConnectionTimedOutETIMEDOUT = 110,
        /**
         * @brief Connection refused (ECONNREFUSED).
         */
        ConnectionRefusedECONNREFUSED = 111,
        /**
         * @brief Host is down (EHOSTDOWN).
         */
        HostIsDownEHOSTDOWN = 112,
        /**
         * @brief No route to host (EHOSTUNREACH).
         */
        NoRouteToHostEHOSTUNREACH = 113,
        /**
         * @brief Operation already in progress (EALREADY).
         */
        OperationAlreadyInProgressEALREADY = 114,
        /**
         * @brief Operation now in progress (EINPROGRESS).
         */
        OperationNowInProgressEINPROGRESS = 115,
        /**
         * @brief Stale file handle (ESTALE).
         */
        StaleFileHandleESTALE = 116,
        /**
         * @brief Structure needs cleaning (EUCLEAN).
         */
        StructureNeedsCleaningEUCLEAN = 117,
        /**
         * @brief Not a XENIX named type file (ENOTNAM).
         */
        NotAXENIXNamedTypeFileENOTNAM = 118,
        /**
         * @brief No XENIX semaphores available (ENAVAIL).
         */
        NoXENIXSemaphoresAvailableENAVAIL = 119,
        /**
         * @brief Is a named type file (EISNAM).
         */
        IsANamedTypeFileEISNAM = 120,
        /**
         * @brief Remote I/O error (EREMOTEIO).
         */
        RemoteIOErrorEREMOTEIO = 121,
        /**
         * @brief Disk quota exceeded (EDQUOT).
         */
        DiskQuotaExceededEDQUOT = 122,
        /**
         * @brief No medium found (ENOMEDIUM).
         */
        NoMediumFoundENOMEDIUM = 123,
        /**
         * @brief Wrong medium type (EMEDIUMTYPE).
         */
        WrongMediumTypeEMEDIUMTYPE = 124,
        /**
         * @brief Operation canceled (ECANCELED).
         */
        OperationCanceledECANCELED = 125,
        /**
         * @brief Required key not available (ENOKEY).
         */
        RequiredKeyNotAvailableENOKEY = 126,
        /**
         * @brief Key has expired (EKEYEXPIRED).
         */
        KeyHasExpiredEKEYEXPIRED = 127,
        /**
         * @brief Key has been revoked (EKEYREVOKED).
         */
        KeyHasBeenRevokedEKEYREVOKED = 128,
        /**
         * @brief Key was rejected by service (EKEYREJECTED).
         */
        KeyWasRejectedByServiceEKEYREJECTED = 129,
        /**
         * @brief Owner died (EOWNERDEAD).
         */
        OwnerDiedEOWNERDEAD = 130,
        /**
         * @brief State not recoverable (ENOTRECOVERABLE).
         */
        StateNotRecoverableENOTRECOVERABLE = 131,
        /**
         * @brief Operation not possible due to RF-kill (ERFKILL).
         */
        OperationNotPossibleDueToRFKillERFKILL = 132,
        /**
         * @brief Memory page has hardware error (EHWPOISON).
         */
        MemoryPageHasHardwareErrorEHWPOISON = 133,
        /**
         * @brief Operation not supported (ENOTSUP).
         */
        OperationNotSupportedENOTSUP = 95
    };

    /**
     * @brief contains the available terminal font weights.
     */
    enum class FontWeight
    {
        /**
         * @brief Usually rendered as bold weight and/or with bright colors.
         */
        Bold = 1,
        /**
         * @brief Usually rendered with faded colors.
         */
        Light
    };

    /**
     * @brief contains the available terminal font layers.
     */
    enum class FontLayer
    {
        /**
         * @brief Refers to the graphemes.
         */
        Foreground = 3,
        /**
         * @brief Refers to the background of the graphemes.
         */
        Background
    };

    /**
     * @brief Represents the available terminal effects.
     */
    enum class FontEffect
    {
        /**
         * @brief Makes the text curly.
         */
        Italic = 1 << 3,
        /**
         * @brief Draws a horizontal line crossing below the text.
         */
        Underline = 1 << 4,
        /**
         * @brief Makes the text blink indefinitely.
         */
        Blink = 1 << 5,
        /**
         * @brief Swaps the background and foreground colors.
         */
        Negative = 1 << 7,
        /**
         * @brief Makes the text hard to see or invisible.
         */
        Hidden = 1 << 8,
        /**
         * @brief Draws a horizontal line crossing through the middle of the text.
         */
        Strike = 1 << 9
    };

    /**
     * @brief contains the ANSI codes of the first 16 colors of the XTerm color palette.
     */
    enum class XColor
    {
        /**
         * @brief The dark variant of the black color.
         */
        DarkBlack,
        /**
         * @brief The dark variant of the red color.
         */
        DarkRed,
        /**
         * @brief The dark variant of the green color.
         */
        DarkGreen,
        /**
         * @brief The dark variant of the yellow color.
         */
        DarkYellow,
        /**
         * @brief The dark variant of the blue color.
         */
        DarkBlue,
        /**
         * @brief The dark variant of the magenta color.
         */
        DarkMagenta,
        /**
         * @brief The dark variant of the cyan color.
         */
        DarkCyan,
        /**
         * @brief The dark variant of the white color.
         */
        DarkWhite,
        /**
         * @brief The light variant of the black color.
         */
        LightBlack,
        /**
         * @brief The light variant of the red color.
         */
        LightRed,
        /**
         * @brief The light variant of the green color.
         */
        LightGreen,
        /**
         * @brief The light variant of the yellow color.
         */
        LightYellow,
        /**
         * @brief The light variant of the blue color.
         */
        LightBlue,
        /**
         * @brief The light variant of the magenta color.
         */
        LightMagenta,
        /**
         * @brief The light variant of the cyan color.
         */
        LightCyan,
        /**
         * @brief The light variant of the white color.
         */
        LightWhite
    };

    /**
     * @brief contains the available terminal cursor shapes.
     */
    enum class CursorShape
    {
        /**
         * @brief Fills the whole character cell.
         */
        Block = 2,
        /**
         * @brief Fills a region at the bottom of the character cell.
         */
        Underline = 4,
        /**
         * @brief Fills a region at the left of the character cell.
         */
        Bar = 6
    };

    /**
     * @brief Represents the available terminal event types.
     */
    enum class EventType
    {
        /**
         * @brief No event is available.
         */
        None,
        /**
         * @brief The event timer ran out.
         */
        TimeOut,
        /**
         * @brief The terminal window was resized.
         */
        Resize,
        /**
         * @brief The terminal window gained/lost focus.
         */
        Focus,
        /**
         * @brief The mouse was moved or clicked.
         */
        Mouse,
        /**
         * @brief A keyboard key was pressed.
         */
        Key
    };

    /**
     * @brief contains the available mouse buttons.
     */
    enum class MouseButton
    {
        /**
         * @brief No button.
         */
        None,
        /**
         * @brief Left mouse button.
         */
        Left,
        /**
         * @brief Mouse wheel button.
         */
        Wheel,
        /**
         * @brief Mouse wheel scroll upwards.
         */
        WheelUp,
        /**
         * @brief Mouse wheel scroll downwards.
         */
        WheelDown,
        /**
         * @brief Right mouse button.
         */
        Right
    };

    /**
     * @brief contains the available keyboard keys.
     */
    enum class KeyboardKey
    {
#ifdef _WIN32
        /**
         * @brief The left arrow key.
         */
        LeftArrow = -23,
        /**
         * @brief The up arrow key.
         */
        UpArrow,
        /**
         * @brief The right arrow key.
         */
        RightArrow,
        /**
         * @brief The down arrow key.
         */
        DownArrow,
#else
        /**
         * @brief The up arrow key.
         */
        UpArrow = -23,
        /**
         * @brief The down arrow key.
         */
        DownArrow,
        /**
         * @brief The right arrow key.
         */
        RightArrow,
        /**
         * @brief The left arrow key.
         */
        LeftArrow,
#endif
        /**
         * @brief The F1 key.
         */
        F1,
        /**
         * @brief The F2 key.
         */
        F2,
        /**
         * @brief The F3 key.
         */
        F3,
        /**
         * @brief The F4 key.
         */
        F4,
        /**
         * @brief The F5 key.
         */
        F5,
        /**
         * @brief The F6 key.
         */
        F6,
        /**
         * @brief The F7 key.
         */
        F7,
        /**
         * @brief The F8 key.
         */
        F8,
        /**
         * @brief The F9 key.
         */
        F9,
        /**
         * @brief The F10 key.
         */
        F10,
        /**
         * @brief The F11 key.
         */
        F11,
        /**
         * @brief The F12 key.
         */
        F12,
#ifdef _WIN32
        /**
         * @brief The PageUp key.
         */
        PageUp,
        /**
         * @brief The PageDown key.
         */
        PageDown,
        /**
         * @brief The End key.
         */
        End,
        /**
         * @brief The Home key.
         */
        Home,
        /**
         * @brief The Insert key.
         */
        Insert,
        /**
         * @brief The Delete key.
         */
        Delete,
#else
        /**
         * @brief The Home key.
         */
        Home,
        /**
         * @brief The Insert key.
         */
        Insert,
        /**
         * @brief The Delete key.
         */
        Delete,
        /**
         * @brief The End key.
         */
        End,
        /**
         * @brief The PageUp key.
         */
        PageUp,
        /**
         * @brief The PageDown key.
         */
        PageDown,
#endif
        /**
         * @brief The Tab key.
         */
        Tab = 9,
#if defined(_WIN32) || defined(__APPLE__)
        /**
         * @brief The Enter key.
         */
        Enter = 13,
#else
        /**
         * @brief The Enter key.
         */
        Enter,
#endif
        /**
         * @brief The Escape key.
         */
        Escape = 27,
        /**
         * @brief The Space bar key.
         */
        SpaceBar = 32,
        /**
         * @brief The Backspace key.
         */
        Backspace = 127
    };

    class RGBColor;

#ifdef _WIN32
    /**
     * @brief Represents an exception thrown whenever an invalid code page tries to be set.
     */
    class InvalidCodePageException final
    {
    };
#endif

    /**
     * @brief Represents an exception thrown whenever invalid stream attributes tries to be set.
     */
    class InvalidStreamAttributesException final
    {
    };

    /**
     * @brief Represents an exception thrown whenever a group of streams are wide character oriented.
     */
    class WideCharacterOrientationException final
    {
    };

    /**
     * @brief Represents an exception thrown whenever a group of streams are not TTY.
     */
    class NoValidTTYException final
    {
    };

    /**
     * @brief Represents an exception thrown whenever a value is out of a certain range.
     */
    class OutOfRangeException final
    {
    };

    /**
     * @brief Represents an exception thrown whenever an invalid event type data tries to be read.
     */
    class InvalidEventTypeException final
    {
    };

    /**
     * @brief Represents the command line arguments.
     */
    class CMDArguments final
    {
    public:
#ifdef _WIN32
        /**
         * @brief Creates an instance of the CMDArguments class.
         * @param totalArguments The total arguments.
         * @param utf16Arguments The arguments in UTF-16 encoding.
         * @param utf8Arguments The arguments in UTF-8 encoding.
         * @returns An instance of the CMDArguments class.
         */
        CMDArguments(int totalArguments, wchar_t** utf16Arguments, char** utf8Arguments);
        /**
         * @brief Destroys an instance of the CMDArguments class.
         */
        ~CMDArguments();
        /**
         * @brief Gets an argument in UTF-16 encoding by using its offset.
         * @param offset The offset to be used.
         * @returns The argument in UTF-16 encoding.
         * @exception OutOfRangeException Thrown whenever the offset is out of the valid range of arguments.
         */
        std::wstring getUTF16ArgumentByOffset(std::size_t offset) const;
#else
        /**
         * @brief Creates an instance of the CMDArguments class.
         * @param totalArguments The total arguments.
         * @param utf8Arguments The arguments in UTF8 encoding.
         * @returns An instance of the CMDArguments class.
         */
        CMDArguments(int totalArguments, char** utf8Arguments);
#endif
        /**
         * @brief Gets the total arguments.
         * @returns The total arguments.
         */
        int getTotalArguments() const;
        /**
         * @brief Gets an argument in UTF-8 encoding by using its offset.
         * @param offset The offset to be used.
         * @returns The argument in UTF-8 encoding.
         * @throws OutOfRangeException Thrown whenever the offset is out of the valid range of arguments.
         */
        std::string getUTF8ArgumentByOffset(std::size_t offset) const;

    private:
#ifdef _WIN32
        /**
         * @brief The arguments in UTF-16 encoding.
         */
        wchar_t** m_utf16Arguments;
#endif
        /**
         * @brief The arguments in UTF-8 encoding.
         */
        char** m_utf8Arguments;
        /**
         * @brief The total arguments.
         */
        int m_totalArguments;
    };

    /**
     * @brief Represents a coordinate inside of the terminal window.
     */
    class Coordinate final
    {
    public:
        /**
         * @brief Creates an instance of the Coordinate class.
         * @returns An instance of the Coordinate class.
         */
        Coordinate();
        /**
         * @brief Creates an instance of the Coordinate class.
         * @param column The column component of the coordinate.
         * @param row The row component of the coordinate.
         * @returns An instance of the Coordinate class.
         */
        Coordinate(unsigned short column, unsigned short row);
        /**
         * @brief Gets the column component of the coordinate.
         * @returns The column component of the coordinate.
         */
        unsigned short getColumn() const;
        /**
         * @brief Gets the row component of the coordinate.
         * @returns The row component of the coordinate.
         */
        unsigned short getRow() const;
        /**
         * @brief Sets the column component of the coordinate.
         * @param column The column component to be set.
         */
        void setColumn(unsigned short column);
        /**
         * @brief Sets the row component of the coordinate.
         * @param row The row component to be set.
         */
        void setRow(unsigned short row);
        /**
         * @brief Gets a string representation of the coordinate.
         * @returns A string representation of the coordinate.
         */
        std::string toString() const;

    private:
        /**
         * @brief The column component of the coordinate.
         */
        unsigned short m_column;
        /**
         * @brief The row component of the coordinate.
         */
        unsigned short m_row;
    };

    /**
     * @brief Represents a region inside of the terminal window.
     */
    class Region final
    {
    public:
        /**
         * @brief Creates an instance of the Region class.
         * @returns An instance of the Region class.
         */
        Region();
        /**
         * @brief Creates an instance of the Region class.
         * @param totalColumns The total columns of the region.
         * @param totalRows The total rows of the region.
         * @returns An instance of the Region class.
         */
        Region(unsigned short totalColumns, unsigned short totalRows);
        /**
         * @brief Creates an instance of the Region class.
         * @param coordinateI The first coordinate of the region.
         * @param coordinateII The second coordinate of the region. It must be opposite to the first one.
         * @returns An instance of the Region class.
         */
        Region(Coordinate coordinateI, Coordinate coordinateII);
        /**
         * @brief Gets the total columns of the region.
         * @returns The total columns of the region.
         */
        unsigned short getTotalColumns() const;
        /**
         * @brief Gets the total rows of the region.
         * @returns The total rows of the region.
         */
        unsigned short getTotalRows() const;
        /**
         * @brief Gets the area of the region.
         * @returns The area of the region.
         */
        unsigned int getArea() const;
        /**
         * @brief Gets the top left coordinate of the region.
         * @returns The top left coordinate of the region.
         */
        Coordinate getTopLeftCoordinate() const;
        /**
         * @brief Gets the top right coordinate of the region.
         * @returns The top right coordinate of the region.
         */
        Coordinate getTopRightCoordinate() const;
        /**
         * @brief Gets the bottom left coordinate of the region.
         * @returns The bottom left coordinate of the region.
         */
        Coordinate getBottomLeftCoordinate() const;
        /**
         * @brief Gets the bottom right coordinate of the region.
         * @returns The bottom right coordinate of the region.
         */
        Coordinate getBottomRightCoordinate() const;
        /**
         * @brief Checks if the region contains a coordinate.
         * @param column The column component of the coordinate.
         * @param row The row component of the coordinate.
         * @returns A boolean that states the region contains the coordinate.
         */
        bool contains(unsigned short column, unsigned short row) const;
        /**
         * @brief Checks if the region contains a coordinate.
         * @param coordinate The coordinate to be checked.
         * @returns A boolean that states the region contains the coordinate.
         */
        bool contains(Coordinate coordinate) const;

    private:
        /**
         * @brief The total columns of the region.
         */
        unsigned short m_totalColumns;
        /**
         * @brief The total rows of the region.
         */
        unsigned short m_totalRows;
        /**
         * @brief The area of the region.
         */
        unsigned int m_area;
        /**
         * @brief The top left coordinate of the region.
         */
        Coordinate m_topLeftCoordinate;
        /**
         * @brief The top right coordinate of the region.
         */
        Coordinate m_topRightCoordinate;
        /**
         * @brief The bottom left coordinate of the region.
         */
        Coordinate m_bottomLeftCoordinate;
        /**
         * @brief The bottom right coordinate of the region.
         */
        Coordinate m_bottomRightCoordinate;
    };

    /**
     * @brief Represents a color in hex format.
     */
    class HexColor final
    {
    public:
        /**
         * @brief Creates an instance of the HexColor class.
         * @param code The hex code of the color. It must be a value in range from 0x0 to 0xffffff.
         * @throws OutOfRangeException Thrown whenever the hex code is out of the valid range.
         * @returns An instance of the HexColor class.
         */
        HexColor(unsigned int code);
        /**
         * @brief Creates an instance of the HexColor class.
         * @param color A color in RGB format to be converted.
         * @returns An instance of the HexColor class.
         */
        HexColor(RGBColor color);
        /**
         * @brief Gets the hex code of the color.
         * @returns The hex code of the color.
         */
        unsigned int getCode() const;
        /**
         * @brief Sets the hex code of the color.
         * @param code The hex code to be set.
         * @throws OutOfRangeException Thrown whenever the hex code is out of the valid range.
         */
        void setCode(unsigned int code);
        /**
         * @brief Gets a string representation of the color.
         * @param hasPrefix A boolean that states the string has the 0x prefix.
         * @param hasZeroPadding A boolean that states the string has a left padding filled by the number zero.
         * @param isUpperCase A boolean that states the string is upper case.
         * @returns A string representation of the color.
         */
        std::string toString(bool hasPrefix, bool hasZeroPadding, bool isUpperCase) const;

    private:
        /**
         * @brief The hex code of the color.
         */
        unsigned int m_code;
    };

    /**
     * @brief Represents a color in RGB format.
     */
    class RGBColor final
    {
    public:
        /**
         * @brief Creates an instance of the RGBColor class.
         * @returns An instance of the RGBColor class.
         */
        RGBColor();
        /**
         * @brief Creates an instance of the RGBColor class.
         * @param red The red component of the color.
         * @param green The green component of the color.
         * @param blue The blue component of the color.
         * @returns An instance of the RGBColor class.
         */
        RGBColor(unsigned char red, unsigned char green, unsigned char blue);
        /**
         * @brief Creates an instance of the RGBColor class.
         * @param color A color in hex format to be converted.
         * @returns An instance of the RGBColor class.
         */
        RGBColor(HexColor color);
        /**
         * @brief Gets the red component of the color.
         * @returns The red component of the color.
         */
        unsigned char getRed() const;
        /**
         * @brief Gets the green component of the color.
         * @returns The green component of the color.
         */
        unsigned char getGreen() const;
        /**
         * @brief Gets the blue component of the color.
         * @returns The blue component of the color.
         */
        unsigned char getBlue() const;
        /**
         * @brief Sets the red component of the color.
         * @param red The red component to be set.
         */
        void setRed(unsigned char red);
        /**
         * @brief Sets the green component of the color.
         * @param green The green component to be set.
         */
        void setGreen(unsigned char green);
        /**
         * @brief Sets the blue component of the color.
         * @param blue The blue component to be set.
         */
        void setBlue(unsigned char blue);
        /**
         * @brief Gets a string representation of the color.
         * @returns A string representation of the color.
         */
        std::string toString() const;

    private:
        /**
         * @brief The red component of the color.
         */
        unsigned char m_red;
        /**
         * @brief The green component of the color.
         */
        unsigned char m_green;
        /**
         * @brief The blue component of the color.
         */
        unsigned char m_blue;
    };

    /**
     * @brief Represents a terminal focus event.
     */
    class FocusEvent final
    {
    public:
        /**
         * @brief Creates an instance of the FocusEvent class.
         * @param hasFocus A boolean that states the terminal window has gained focus.
         * @returns An instance of the FocusEvent class.
         */
        FocusEvent(bool hasFocus);
        /**
         * @brief Checks if the terminal window has gained focus.
         * @returns A boolean that states the terminal window has gained focus.
         */
        bool hasFocus() const;

    private:
        /**
         * @brief A boolean that states the terminal window has gained focus.
         */
        bool m_hasFocus;
    };

    /**
     * @brief Represents a terminal window resize event.
     */
    class ResizeEvent final
    {
    public:
        /**
         * @brief Creates an instance of the ResizeEvent class.
         * @returns An instance of the ResizeEvent class.
         */
        ResizeEvent();
        /**
         * @brief Gets the terminal window region.
         * @returns The terminal window region.
         */
        Region getRegion() const;

    private:
        /**
         * @brief The terminal window region.
         */
        Region m_region;
    };

    /**
     * @brief Represents a terminal mouse event.
     */
    class MouseEvent final
    {
    public:
        /**
         * @brief Creates an instance of the MouseEvent class.
         * @param coordinate The mouse coordinate.
         * @param button The mouse button pressed.
         * @param isDragging A boolean that states the mouse was being dragged.
         * @param hasCtrl A boolean that states the Ctrl key was being holded.
         * @param hasAlt A boolean that states the Alt key was being holded.
         * @param hasShift A boolean that states the Shift key was being holded.
         * @returns An instance of the MouseEvent class.
         */
        MouseEvent(Coordinate coordinate, MouseButton button, bool isDragging, bool hasCtrl, bool hasAlt, bool hasShift);
        /**
         * @brief Gets the mouse coordinate.
         * @returns The mouse coordinate.
         */
        Coordinate getCoordinate() const;
        /**
         * @brief Gets the mouse button pressed.
         * @returns The mouse button pressed.
         */
        MouseButton getButton() const;
        /**
         * @brief Checks if the mouse was being dragged.
         * @returns A boolean that states the mouse was being dragged.
         */
        bool isDragging() const;
        /**
         * @brief Checks if the Ctrl was being holded.
         * @returns A boolean that states the Ctrl key was being holded.
         */
        bool hasCtrl() const;
        /**
         * @brief Checks if the Alt was being holded.
         * @returns A boolean that states the Alt key was being holded.
         */
        bool hasAlt() const;
        /**
         * @brief Checks if the Shift was being holded.
         * @returns A boolean that states the Shift key was being holded.
         */
        bool hasShift() const;

    private:
        /**
         * @brief The mouse coordinate.
         */
        Coordinate m_coordinate;
        /**
         * @brief The mouse button pressed.
         */
        MouseButton m_button;
        /**
         * @brief A boolean that states the mouse was being dragged.
         */
        bool m_isDragging;
        /**
         * @brief A boolean that states the Ctrl key was being holded.
         */
        bool m_hasCtrl;
        /**
         * @brief A boolean that states the Alt key was being holded.
         */
        bool m_hasAlt;
        /**
         * @brief A boolean that states the Shift key was being holded.
         */
        bool m_hasShift;
    };

    /**
     * @brief Represents a terminal key event.
     */
    class KeyEvent final
    {
    public:
        /**
         * @brief Creates an instance of the KeyEvent class.
         * @param key The key pressed. It may be an UTF-8 grapheme or an enumerator from the KeyboardKey enum class.
         * @param hasCtrl A boolean that states the Ctrl key was being holded.
         * @param hasAlt A boolean that states the Alt key was being holded.
         * @param hasShift A boolean that states the Shift key was being holded.
         * @returns An instance of the KeyEvent class.
         */
        KeyEvent(int key, bool hasCtrl, bool hasAlt, bool hasShift);
        /**
         * @brief Gets the key pressed.
         * @returns The key pressed. It may be an UTF-8 grapheme or an enumerator from the KeyboardKey enum class.
         */
        int getKey() const;
        /**
         * @brief Checks if the Ctrl key was being holded.
         * @returns A boolean that states the Ctrl key was being holded.
         */
        bool hasCtrl() const;
        /**
         * @brief Checks if the Alt was being holded.
         * @returns A boolean that states the Alt key was being holded.
         */
        bool hasAlt() const;
        /**
         * @brief Checks if the Shift was being holded.
         * @returns A boolean that states the Shift key was being holded.
         */
        bool hasShift() const;

    private:
        /**
         * @brief The key pressed. It may be an UTF-8 grapheme or an enumerator from the KeyboardKey enum class.
         */
        int m_key;
        /**
         * @brief A boolean that states the Ctrl key was being holded.
         */
        bool m_hasCtrl;
        /**
         * @brief A boolean that states the Alt key was being holded.
         */
        bool m_hasAlt;
        /**
         * @brief A boolean that states the Shift key was being holded.
         */
        bool m_hasShift;
    };

    /**
     * @brief Represents the information of a terminal event.
     */
    class EventInfo final
    {
    public:
        /**
         * @brief Creates an instance of the EventInfo class.
         * @param type The event type.
         * @returns An instance of the EventInfo class.
         */
        EventInfo(EventType type);
        /**
         * @brief Creates an instance of the EventInfo class.
         * @param focusEvent A focus event to be converted.
         * @returns An instance of the EventInfo class.
         */
        EventInfo(FocusEvent focusEvent);
        /**
         * @brief Creates an instance of the EventInfo class.
         * @param resizeEvent A resize event to be converted.
         * @returns An instance of the EventInfo class.
         */
        EventInfo(ResizeEvent resizeEvent);
        /**
         * @brief Creates an instance of the EventInfo class.
         * @param mouseEvent A mouse event to be converted.
         * @returns An instance of the EventInfo class.
         */
        EventInfo(MouseEvent mouseEvent);
        /**
         * @brief Creates an instance of the EventInfo class.
         * @param keyEvent A key event to be converted.
         * @returns An instance of the EventInfo class.
         */
        EventInfo(KeyEvent keyEvent);
        /**
         * @brief Gets the event type.
         * @returns The event type.
         */
        EventType getType() const;
        /**
         * @brief Gets the focus event read.
         * @returns The focus event read.
         * @throws InvalidEventTypeException Thrown whenever the event type is not a focus event.
         */
        FocusEvent getFocusEvent() const;
        /**
         * @brief Gets the resize event read.
         * @returns The resize event read.
         * @throws InvalidEventTypeException Thrown whenever the event type is not a resize event.
         */
        ResizeEvent getResizeEvent() const;
        /**
         * @brief Gets the mouse event read.
         * @returns The mouse event read.
         * @throws InvalidEventTypeException Thrown whenever the event type is not a mouse event.
         */
        MouseEvent getMouseEvent() const;
        /**
         * @brief Gets the key event read.
         * @returns The key event read.
         * @throws InvalidEventTypeException Thrown whenever the event type is not a key event.
         */
        KeyEvent getKeyEvent() const;

    private:
        /**
         * @brief The event type.
         */
        EventType m_type;
        union {
            /**
             * @brief The focus event read.
             */
            FocusEvent m_focusEvent;
            /**
             * @brief The resize event read.
             */
            ResizeEvent m_resizeEvent;
            /**
             * @brief The mouse event read.
             */
            MouseEvent m_mouseEvent;
            /**
             * @brief The key event read.
             */
            KeyEvent m_keyEvent;
        };
    };

    /**
     * @brief Represents the terminal.
     */
    class Terminal final
    {
    public:
#ifdef _WIN32
        /**
         * @brief Represents the terminal encoding.
         */
        class Encoding final
        {
        public:
            /**
             * @brief Converts an UTF-16 encoded string to UTF-8.
             * @param utf16String The UTF-16 encoded string to be converted.
             * @returns The converted UTF-8 encoded string.
             */
            static std::string convertUTF16ToUTF8(std::wstring utf16String);
            /**
             * @brief Converts an UTF-8 encoded string to UTF-16.
             * @param utf8String The UTF-8 encoded string to be converted.
             * @returns The converted UTF-16 encoded string.
             */
            static std::wstring convertUTF8ToUTF16(std::string utf8String);

        private:
            Encoding() = delete;
        };
#endif

        /**
         * @brief Represents the standard input stream.
         */
        class Input final
        {
        public:
            /**
             * @brief Clears the standard input buffer.
             */
            static void clear();
            /**
             * @brief Checks if the standard input stream is connected to an interactive terminal (TTY).
             * @param A boolean that states the standard input stream is a TTY.
             */
            static bool isTTY();
            /**
             * @brief Reads a terminal event.
             * @param allowMouseCapture A boolean that states mouse events should be captured. If enabled, mouse selection will be disabled until it returns.
             * @returns The information about the event read.
             */
            static EventInfo readEvent(bool allowMouseCapture);
            /**
             * @brief Reads a terminal event.
             * @param allowMouseCapture A boolean that states mouse events should be captured. If enabled, mouse selection will be disabled until it returns.
             * @param wait The time to wait for an event. If zero, it returns immediately.
             * @returns The information about the event read.
             */
            static EventInfo readEvent(bool allowMouseCapture, std::chrono::milliseconds wait);
            /**
             * @brief Reads a terminal event.
             * @param allowMouseCapture A boolean that states mouse events should be captured. If enabled, mouse selection will be disabled until it returns.
             * @param wait The time to wait for an event. If zero, it returns immediately.
             * @param filter A function to be used to filter events while the timer is running.
             * @returns The information about the event read.
             */
            static EventInfo readEvent(bool allowMouseCapture, std::chrono::milliseconds wait, std::function<bool(EventInfo&)> filter);

        private:
            Input() = delete;
        };

        /**
         * @brief Represents the standard output stream.
         */
        class Output final
        {
        public:
            /**
             * @brief Flushes the standard output stream buffer.
             */
            static void flush();
            /**
             * @brief Formats and writes arguments to the standard output stream with a newline character appended at its end.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param arguments The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard output stream is wide character oriented.
             */
            static void writeLine(std::string format, std::va_list arguments);
            /**
             * @brief Formats and writes arguments to the standard output stream with a newline character appended at its end.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard output stream is wide character oriented.
             */
            static void writeLine(std::string format, ...);
            /**
             * @brief Writes a newline character to the standard output stream.
             * @throws WideCharacterOrientationException Thrown whenever the standard output stream is wide character oriented.
             */
            static void writeLine();
            /**
             * @brief Formats and writes arguments to the standard output stream.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param arguments The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard output stream is wide character oriented.
             */
            static void write(std::string format, std::va_list arguments);
            /**
             * @brief Formats and writes arguments to the standard output stream.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard output stream is wide character oriented.
             */
            static void write(std::string format, ...);
            /**
             * @brief Checks if the standard output stream is connected to an interactive terminal (TTY).
             * @returns A boolean that states the standard output stream is a TTY.
             */
            static bool isTTY();

        private:
            Output() = delete;
        };

        /**
         * @brief Represents the standard error stream.
         */
        class Error final
        {
        public:
            /**
             * @brief Formats and writes arguments to the standard error stream with a newline character appended at its end.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param arguments The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard error stream is wide character oriented.
             */
            static void writeLine(std::string format, std::va_list arguments);
            /**
             * @brief Formats and writes arguments to the standard error stream with a newline character appended at its end.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard error stream is wide character oriented.
             */
            static void writeLine(std::string format, ...);
            /**
             * @brief Writes a newline character to the standard error stream.
             * @throws WideCharacterOrientationException Thrown whenever the standard error stream is wide character oriented.
             */
            static void writeLine();
            /**
             * @brief Formats and writes arguments to the standard error stream.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param arguments The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard error stream is wide character oriented.
             */
            static void write(std::string format, std::va_list arguments);
            /**
             * @brief Formats and writes arguments to the standard error stream.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard error stream is wide character oriented.
             */
            static void write(std::string format, ...);
            /**
             * @brief Checks if the standard error stream is connected to an interactive terminal (TTY).
             * @returns A boolean that states the standard error stream is a TTY.
             */
            static bool isTTY();

        private:
            Error() = delete;
        };

        /**
         * @brief Represents the terminal process.
         */
        class Process final
        {
        public:
            /**
             * @brief Gets and treats the command line arguments.
             * @param rawTotalArguments The total arguments given as first parameter of the main function.
             * @param rawArguments The arguments given as second parameter of the main function.
             * @returns The treated arguments.
             */
            static CMDArguments getCMDArguments(int totalRawCMDArguments, char** rawCMDArguments);
            /**
             * @brief Exits the process.
             * @param exitCode The exit code to be used. It must be a value in range from 0 to 255.
             * @throws OutOfRangeException Thrown whenever the exit code is out of the valid range.
             */
            static void exit(int exitCode);
            /**
             * @brief Exits the process.
             * @param exitCode The exit code to be used.
             */
            static void exit(ExitCode exitCode);

        private:
            Process() = delete;
        };

        /**
         * @brief Represents the terminal window.
         */
        class Window final
        {
        public:
            /**
             * @brief Gets the terminal window geometry.
             * @returns The terminal window geometry.
             */
            static Region getRegion();
            /**
             * @brief Opens the alternate window.
             */
            static void openAlternateWindow();
            /**
             * @brief Closes the alternate window.
             */
            static void closeAlternateWindow();
            /**
             * @brief Sets the terminal window title.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param arguments The arguments to be formatted.
             */
            static void setTitle(std::string format, std::va_list arguments);
            /**
             * @brief Sets the terminal window title.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             */
            static void setTitle(std::string format, ...);

        private:
            Window() = delete;
        };

        /**
         * @brief Represents the terminal bell.
         */
        class Bell final
        {
        public:
            /**
             * @brief Rings the terminal bell.
             */
            static void ring();

        private:
            Bell() = delete;
        };

        /**
         * @brief Represents the terminal font.
         */
        class Font final
        {
        public:
            /**
             * @brief Sets the terminal font weight.
             * @param weight The weight to be set.
             */
            static void setWeight(FontWeight weight);
            /**
             * @brief Sets an XTerm color into the terminal font.
             * @param color The ANSI code of the color.
             * @param layer The layer of the font to be affected.
             */
            static void setXColor(unsigned char color, FontLayer layer);
            /**
             * @brief Sets an XTerm color into the terminal font.
             * @param color The ANSI code of the color.
             * @param layer The layer of the font to be affected.
             */
            static void setXColor(XColor color, FontLayer layer);
            /**
             * @brief Sets an RGB color into the terminal font.
             * @param red The red component of the color.
             * @param green The green component of the color.
             * @param blue The blue component of the color.
             * @param layer The layer of the font to be affected.
             */
            static void setRGBColor(unsigned char red, unsigned char green, unsigned char blue, FontLayer layer);
            /**
             * @brief Sets an RGB color into the terminal font.
             * @param color The color to be applied.
             * @param layer The layer of the font to be affected.
             */
            static void setRGBColor(RGBColor color, FontLayer layer);
            /**
             * @brief Sets a hex color into the terminal font.
             * @param color The hex code of the color.
             * @param layer The layer of the font to be affected.
             */
            static void setHexColor(unsigned int hex, FontLayer layer);
            /**
             * @brief Sets a hex color into the terminal font.
             * @param color The color to be applied.
             * @param layer The layer of the font to be affected.
             */
            static void setHexColor(HexColor color, FontLayer layer);
            /**
             * @brief Sets the terminal effects flagged in a bitmask.
             * @param effect The bitmask containing the effects. It must be composed by enumerators from the FontEffect enum class.
             * @throws OutOfRangeException Thrown whenever an invalid effect is used to compose the bitmask.
             */
            static void setEffects(int effect);
            /**
             * @brief Sets a terminal effect.
             * @param effect The effect to be set.
             */
            static void setEffects(FontEffect effect);
            /**
             * @brief Resets the terminal font colors.
             */
            static void resetColors();
            /**
             * @brief Resets the terminal font weight.
             */
            static void resetWeight();
            /**
             * @brief Resets the terminal effects.
             */
            static void resetEffects();

        private:
            Font() = delete;
        };

        /**
         * @brief Represents the terminal cursor.
         */
        class Cursor final
        {
        public:
            /**
             * @brief Gets the terminal cursor coordinate. On Linux, as it parses a terminal answer given through the standard input stream, its buffer will always be cleared.
             * @returns The terminal cursor coordinate.
             * @throws NoValidTTYException Thrown, on Windows, whenever the standard output and error streams are not TTY; and, on Linux, whenever the standard input or the
             * standard output and error streams are not TTY.
             * @throws WideCharacterOrientationException Thrown, on Linux, whenever the standard input stream is wide character oriented.
             */
            static Coordinate getCoordinate();
            /**
             * @brief Sets the terminal cursor coordinate.
             * @param column The column component of the coordinate.
             * @param row The row component of the coordinate.
             * @throws OutOfRangeException Thrown whenever the coordinate is outside of the terminal window boundaries.
             */
            static void setCoordinate(unsigned short column, unsigned short row);
            /**
             * @brief Sets the terminal cursor coordinate.
             * @param coordinate The coordinate to be set.
             * @throws OutOfRangeException Thrown whenever the coordinate is outside of the terminal window boundaries.
             */
            static void setCoordinate(Coordinate coordinate);
            /**
             * @brief Sets the terminal cursor shape.
             * @param shape The shape to be set.
             */
            static void setShape(CursorShape shape, bool isBlinking);
            /**
             * @brief Sets the terminal cursor visibility.
             * @param isVisible A boolean that states the cursor should be visible.
             */
            static void setVisibility(bool isVisible);
            /**
             * @brief Resets the terminal cursor shape.
             */
            static void resetShape();
            /**
             * @brief Clears the terminal cursor line.
             */
            static void clearLine();

        private:
            Cursor() = delete;
        };

    private:
        Terminal() = delete;
    };

    /**
     * @brief Concatenates two effects together in order to create a bitmask.
     * @param effectI The first effect.
     * @param effectII The second effect.
     * @returns A bitmask containing both effects.
     */
    int operator|(FontEffect effectI, FontEffect effectII);
    /**
     * @brief Checks if a code and a keyboard key are equal.
     * @param code The code to be checked.
     * @param key The key to be used as reference.
     * @returns A boolean that states both are equal.
     */
    bool operator==(int code, KeyboardKey key);
    /**
     * @brief Checks if a keyboard key and a code are equal.
     * @param key The key to be checked.
     * @param code The code to be used as reference.
     * @returns A boolean that states both are equal.
     */
    bool operator==(KeyboardKey key, int code);
    /**
     * @brief Checks if a code and a keyboard key are different.
     * @param code The code to be checked.
     * @param key The key to be used as reference.
     * @returns A boolean that states both are different.
     */
    bool operator!=(int code, KeyboardKey key);
    /**
     * @brief Checks if a keyboard key and a code are different.
     * @param key The key to be checked.
     * @param code The code to be used as reference.
     * @returns A boolean that states both are different.
     */
    bool operator!=(KeyboardKey key, int code);
    /**
     * @brief Checks if two hex colors are equal.
     * @param colorI The first color.
     * @param colorII The second color.
     * @returns A boolean that states both colors are equal.
     */
    bool operator==(HexColor colorI, HexColor colorII);
    /**
     * @brief Checks if two hex colors are different.
     * @param colorI The first color.
     * @param colorII The second color.
     * @returns A boolean that states both colors are different.
     */
    bool operator!=(HexColor colorI, HexColor colorII);
    /**
     * @brief Checks if two RGB colors are equal.
     * @param colorI The first color.
     * @param colorII The second color.
     * @returns A boolean that states both colors are equal.
     */
    bool operator==(RGBColor colorI, RGBColor colorII);
    /**
     * @brief Checks if two RGB colors are different.
     * @param colorI The first color.
     * @param colorII The second color.
     * @returns A boolean that states both colors are different.
     */
    bool operator!=(RGBColor colorI, RGBColor colorII);
    /**
     * @brief Checks if a hex color is equal to an RGB one.
     * @param hexColor The hex color.
     * @param rgbColor The RGB color.
     * @returns A boolean that states both colors are equal.
     */
    bool operator==(HexColor hexColor, RGBColor rgbColor);
    /**
     * @brief Checks if a hex color is different to an RGB one.
     * @param hexColor The hex color.
     * @param rgbColor The RGB color.
     * @returns A boolean that states both colors are different.
     */
    bool operator!=(HexColor hexColor, RGBColor rgbColor);
    /**
     * @brief Checks if an RGB color is equal to a hex one.
     * @param rgbColor The RGB color.
     * @param hexColor The hex color.
     * @returns A boolean that states both colors are equal.
     */
    bool operator==(RGBColor rgbColor, HexColor hexColor);
    /**
     * @brief Checks if an RGB color is different to a hex one.
     * @param rgbColor The RGB color.
     * @param hexColor The hex color.
     * @returns A boolean that states both colors are different.
     */
    bool operator!=(RGBColor rgbColor, HexColor hexColor);
}
