#pragma once

#include <chrono>
#include <cstdarg>
#include <functional>
#include <memory>
#include <string>

using namespace std::literals::chrono_literals;

/// <summary>
/// Terminal Manipulation Kit (TMK) - An open-source C++ terminal manipulation library for Windows and Linux that includes features to handle terminal attributes, arguments and
/// events targetting UTF-8 encoding.
/// </summary>
namespace TMK
{
    /// <summary>
    /// Contains the POSIX exit codes.
    /// </summary>
    enum class ExitCode
    {
        /// <summary>
        /// Successful execution.
        /// </summary>
        Success,
        /// <summary>
        /// Generic failed execution.
        /// </summary>
        Failure,
        /// <summary>
        /// Operation not permitted (EPERM).
        /// </summary>
        OperationNotPermittedEPERM = 1,
        /// <summary>
        /// No such file or directory (ENOENT).
        /// </summary>
        NoSuchFileOrDirectoryENOENT,
        /// <summary>
        /// No such process (ESRCH).
        /// </summary>
        NoSuchProcessESRCH,
        /// <summary>
        /// Interrupted system call (EINTR).
        /// </summary>
        InterruptedSystemCallEINTR,
        /// <summary>
        /// Input/output error (EIO).
        /// </summary>
        InputOutputErrorEIO,
        /// <summary>
        /// No such device or address (ENXIO).
        /// </summary>
        NoSuchDeviceOrAddressENXIO,
        /// <summary>
        /// Argument list too long (E2BIG).
        /// </summary>
        ArgumentListTooLongE2BIG,
        /// <summary>
        /// Exec format error (ENOEXEC).
        /// </summary>
        ExecFormatErrorENOEXEC,
        /// <summary>
        /// Bad file descriptor (EBADF).
        /// </summary>
        BadFileDescriptorEBADF,
        /// <summary>
        /// No child processes (ECHILD).
        /// </summary>
        NoChildProcessesECHILD,
        /// <summary>
        /// Resource temporarily unavailable (EAGAIN).
        /// </summary>
        ResourceTemporarilyUnavailableEAGAIN,
        /// <summary>
        /// Resource temporarily unavailable (EWOULDBLOCK).
        /// </summary>
        ResourceTemporarilyUnavailableEWOULDBLOCK = 11,
        /// <summary>
        /// Can not allocate memory (ENOMEM).
        /// </summary>
        CanNotAllocateMemoryENOMEM,
        /// <summary>
        /// Permission denied (EACCES).
        /// </summary>
        PermissionDeniedEACCES,
        /// <summary>
        /// Bad address (EFAULT).
        /// </summary>
        BadAddressEFAULT,
        /// <summary>
        /// Block device required (ENOTBLK).
        /// </summary>
        BlockDeviceRequiredENOTBLK,
        /// <summary>
        /// Device or resource busy (EBUSY).
        /// </summary>
        DeviceOrResourceBusyEBUSY,
        /// <summary>
        /// File exists (EEXIST).
        /// </summary>
        FileExistsEEXIST,
        /// <summary>
        /// Invalid cross-device link (EXDEV).
        /// </summary>
        InvalidCrossDeviceLinkEXDEV,
        /// <summary>
        /// No such device (ENODEV).
        /// </summary>
        NoSuchDeviceENODEV,
        /// <summary>
        /// Not a directory (ENOTDIR).
        /// </summary>
        NotADirectoryENOTDIR,
        /// <summary>
        /// Is a directory (EISDIR).
        /// </summary>
        IsADirectoryEISDIR,
        /// <summary>
        /// Invalid argument (EINVAL).
        /// </summary>
        InvalidArgumentEINVAL,
        /// <summary>
        /// Too many open files in system (ENFILE).
        /// </summary>
        TooManyOpenFilesInSystemENFILE,
        /// <summary>
        /// Too many open files (EMFILE).
        /// </summary>
        TooManyOpenFilesEMFILE,
        /// <summary>
        /// Inappropriate ioctl for device (ENOTTY).
        /// </summary>
        InappropriateIoctlForDeviceENOTTY,
        /// <summary>
        /// Text file busy (ETXTBSY).
        /// </summary>
        TextFileBusyETXTBSY,
        /// <summary>
        /// File too large (EFBIG).
        /// </summary>
        FileTooLargeEFBIG,
        /// <summary>
        /// No space left on device (ENOSPC).
        /// </summary>
        NoSpaceLeftOnDeviceENOSPC,
        /// <summary>
        /// Illegal seek (ESPIPE).
        /// </summary>
        IllegalSeekESPIPE,
        /// <summary>
        /// Read-only file system (EROFS).
        /// </summary>
        ReadOnlyFileSystemEROFS,
        /// <summary>
        /// Too many links (EMLINK).
        /// </summary>
        TooManyLinksEMLINK,
        /// <summary>
        /// Broken pipe (EPIPE).
        /// </summary>
        BrokenPipeEPIPE,
        /// <summary>
        /// Numerical argument out of domain (EDOM).
        /// </summary>
        NumericalArgumentOutOfDomainEDOM,
        /// <summary>
        /// Numerical result out of range (ERANGE).
        /// </summary>
        NumericalResultOutOfRangeERANGE,
        /// <summary>
        /// Resource deadlock avoided (EDEADLK).
        /// </summary>
        ResourceDeadlockAvoidedEDEADLK,
        /// <summary>
        /// Resource deadlock avoided (EDEADLOCK).
        /// </summary>
        ResourceDeadlockAvoidedEDEADLOCK = 35,
        /// <summary>
        /// File name too long (ENAMETOOLONG).
        /// </summary>
        FileNameTooLongENAMETOOLONG,
        /// <summary>
        /// No locks available (ENOLCK).
        /// </summary>
        NoLocksAvailableENOLCK,
        /// <summary>
        /// Function not implemented (ENOSYS).
        /// </summary>
        FunctionNotImplementedENOSYS,
        /// <summary>
        /// Directory not empty (ENOTEMPTY).
        /// </summary>
        DirectoryNotEmptyENOTEMPTY,
        /// <summary>
        /// Too many levels of symbolic links (ELOOP).
        /// </summary>
        TooManyLevelsOfSymbolicLinksELOOP,
        /// <summary>
        /// No message of desired type (ENOMSG).
        /// </summary>
        NoMessageOfDesiredTypeENOMSG = 42,
        /// <summary>
        /// Identifier removed (EIDRM).
        /// </summary>
        IdentifierRemovedEIDRM,
        /// <summary>
        /// Channel number out of range (ECHRNG).
        /// </summary>
        ChannelNumberOutOfRangeECHRNG,
        /// <summary>
        /// Level 2 not synchronized (EL2NSYNC).
        /// </summary>
        Level2NotSynchronizedEL2NSYNC,
        /// <summary>
        /// Level 3 halted (EL3HLT).
        /// </summary>
        Level3HaltedEL3HLT,
        /// <summary>
        /// Level 3 reset (EL3RST).
        /// </summary>
        Level3ResetEL3RST,
        /// <summary>
        /// Link number out of range (ELNRNG).
        /// </summary>
        LinkNumberOutOfRangeELNRNG,
        /// <summary>
        /// Protocol driver not attached (EUNATCH).
        /// </summary>
        ProtocolDriverNotAttachedEUNATCH,
        /// <summary>
        /// No CSI structure available (ENOCSI).
        /// </summary>
        NoCSIStructureAvailableENOCSI,
        /// <summary>
        /// Level 2 halted (EL2HLT).
        /// </summary>
        Level2HaltedEL2HLT,
        /// <summary>
        /// Invalid exchange (EBADE).
        /// </summary>
        InvalidExchangeEBADE,
        /// <summary>
        /// Invalid request descriptor (EBADR).
        /// </summary>
        InvalidRequestDescriptorEBADR,
        /// <summary>
        /// Exchange full (EXFULL).
        /// </summary>
        ExchangeFullEXFULL,
        /// <summary>
        /// No anode (ENOANO).
        /// </summary>
        NoAnodeENOANO,
        /// <summary>
        /// Invalid request code (EBADRQC).
        /// </summary>
        InvalidRequestCodeEBADRQC,
        /// <summary>
        /// Invalid slot (EBADSLT).
        /// </summary>
        InvalidSlotEBADSLT,
        /// <summary>
        /// Bad font file format (EBFONT).
        /// </summary>
        BadFontFileFormatEBFONT = 59,
        /// <summary>
        /// Device not a stream (ENOSTR).
        /// </summary>
        DeviceNotAStreamENOSTR,
        /// <summary>
        /// No data available (ENODATA).
        /// </summary>
        NoDataAvailableENODATA,
        /// <summary>
        /// Timer expired (ETIME).
        /// </summary>
        TimerExpiredETIME,
        /// <summary>
        /// Out of streams resources (ENOSR).
        /// </summary>
        OutOfStreamsResourcesENOSR,
        /// <summary>
        /// Machine is not on the network (ENONET).
        /// </summary>
        MachineIsNotOnTheNetworkENONET,
        /// <summary>
        /// Package not installed (ENOPKG).
        /// </summary>
        PackageNotInstalledENOPKG,
        /// <summary>
        /// Object is remote (EREMOTE).
        /// </summary>
        ObjectIsRemoteEREMOTE,
        /// <summary>
        /// Link has been severed (ENOLINK).
        /// </summary>
        LinkHasBeenSeveredENOLINK,
        /// <summary>
        /// Advertise error (EADV).
        /// </summary>
        AdvertiseErrorEADV,
        /// <summary>
        /// Srmount error (ESRMNT).
        /// </summary>
        SrmountErrorESRMNT,
        /// <summary>
        /// Communication error on send (ECOMM).
        /// </summary>
        CommunicationErrorOnSendECOMM,
        /// <summary>
        /// Protocol error (EPROTO).
        /// </summary>
        ProtocolErrorEPROTO,
        /// <summary>
        /// Multihop attempted (EMULTIHOP).
        /// </summary>
        MultihopAttemptedEMULTIHOP,
        /// <summary>
        /// RFS specific error (EDOTDOT).
        /// </summary>
        RFSSpecificErrorEDOTDOT,
        /// <summary>
        /// Bad message (EBADMSG).
        /// </summary>
        BadMessageEBADMSG,
        /// <summary>
        /// Value too large for defined data type (EOVERFLOW).
        /// </summary>
        ValueTooLargeForDefinedDataTypeEOVERFLOW,
        /// <summary>
        /// Name not unique on network (ENOTUNIQ).
        /// </summary>
        NameNotUniqueOnNetworkENOTUNIQ,
        /// <summary>
        /// File descriptor in bad state (EBADFD).
        /// </summary>
        FileDescriptorInBadStateEBADFD,
        /// <summary>
        /// Remote address changed (EREMCHG).
        /// </summary>
        RemoteAddressChangedEREMCHG,
        /// <summary>
        /// Can not access a needed shared library (ELIBACC).
        /// </summary>
        CanNotAccessANeededSharedLibraryELIBACC,
        /// <summary>
        /// Accessing a corrupted shared library (ELIBBAD).
        /// </summary>
        AccessingACorruptedSharedLibraryELIBBAD,
        /// <summary>
        /// .lib section in a.out corrupted (ELIBSCN).
        /// </summary>
        LibSectionInAOutCorruptedELIBSCN,
        /// <summary>
        /// Attempting to link in too many shared libraries (ELIBMAX).
        /// </summary>
        AttemptingToLinkInTooManySharedLibrariesELIBMAX,
        /// <summary>
        /// Cannot exec a shared library directly (ELIBEXEC).
        /// </summary>
        CannotExecASharedLibraryDirectlyELIBEXEC,
        /// <summary>
        /// Invalid or incomplete multibyte or wide character (EILSEQ).
        /// </summary>
        InvalidOrIncompleteMultibyteOrWideCharacterEILSEQ,
        /// <summary>
        /// Interrupted system call should be restarted (ERESTART).
        /// </summary>
        InterruptedSystemCallShouldBeRestartedERESTART,
        /// <summary>
        /// Streams pipe error (ESTRPIPE).
        /// </summary>
        StreamsPipeErrorESTRPIPE,
        /// <summary>
        /// Too many users (EUSERS).
        /// </summary>
        TooManyUsersEUSERS,
        /// <summary>
        /// Socket operation on non-socket (ENOTSOCK).
        /// </summary>
        SocketOperationOnNonSocketENOTSOCK,
        /// <summary>
        /// Destination address required (EDESTADDRREQ).
        /// </summary>
        DestinationAddressRequiredEDESTADDRREQ,
        /// <summary>
        /// Message too long (EMSGSIZE).
        /// </summary>
        MessageTooLongEMSGSIZE,
        /// <summary>
        /// Protocol wrong type for socket (EPROTOTYPE).
        /// </summary>
        ProtocolWrongTypeForSocketEPROTOTYPE,
        /// <summary>
        /// Protocol not available (ENOPROTOOPT).
        /// </summary>
        ProtocolNotAvailableENOPROTOOPT,
        /// <summary>
        /// Protocol not supported (EPROTONOSUPPORT).
        /// </summary>
        ProtocolNotSupportedEPROTONOSUPPORT,
        /// <summary>
        /// Socket type not supported (ESOCKTNOSUPPORT).
        /// </summary>
        SocketTypeNotSupportedESOCKTNOSUPPORT,
        /// <summary>
        /// Operation not supported (EOPNOTSUPP).
        /// </summary>
        OperationNotSupportedEOPNOTSUPP,
        /// <summary>
        /// Operation not supported (ENOTSUP).
        /// </summary>
        OperationNotSupportedENOTSUP = 95,
        /// <summary>
        /// Protocol family not supported (EPFNOSUPPORT).
        /// </summary>
        ProtocolFamilyNotSupportedEPFNOSUPPORT,
        /// <summary>
        /// Address family not supported by protocol (EAFNOSUPPORT).
        /// </summary>
        AddressFamilyNotSupportedByProtocolEAFNOSUPPORT,
        /// <summary>
        /// Address already in use (EADDRINUSE).
        /// </summary>
        AddressAlreadyInUseEADDRINUSE,
        /// <summary>
        /// Cannot assign requested address (EADDRNOTAVAIL).
        /// </summary>
        CannotAssignRequestedAddressEADDRNOTAVAIL,
        /// <summary>
        /// Network is down (ENETDOWN).
        /// </summary>
        NetworkIsDownENETDOWN,
        /// <summary>
        /// Network is unreachable (ENETUNREACH).
        /// </summary>
        NetworkIsUnreachableENETUNREACH,
        /// <summary>
        /// Network dropped connection on reset (ENETRESET).
        /// </summary>
        NetworkDroppedConnectionOnResetENETRESET,
        /// <summary>
        /// Software caused connection abort (ECONNABORTED).
        /// </summary>
        SoftwareCausedConnectionAbortECONNABORTED,
        /// <summary>
        /// Connection reset by peer (ECONNRESET).
        /// </summary>
        ConnectionResetByPeerECONNRESET,
        /// <summary>
        /// No buffer space available (ENOBUFS).
        /// </summary>
        NoBufferSpaceAvailableENOBUFS,
        /// <summary>
        /// Transport endpoint is already connected (EISCONN).
        /// </summary>
        TransportEndpointIsAlreadyConnectedEISCONN,
        /// <summary>
        /// Transport endpoint is not connected (ENOTCONN).
        /// </summary>
        TransportEndpointIsNotConnectedENOTCONN,
        /// <summary>
        /// Can not send after transport endpoint shutdown (ESHUTDOWN).
        /// </summary>
        CanNotSendAfterTransportEndpointShutdownESHUTDOWN,
        /// <summary>
        /// Too many references: cannot splice (ETOOMANYREFS).
        /// </summary>
        TooManyReferencesCannotSpliceETOOMANYREFSs,
        /// <summary>
        /// Connection timed out (ETIMEDOUT).
        /// </summary>
        ConnectionTimedOutETIMEDOUT,
        /// <summary>
        /// Connection refused (ECONNREFUSED).
        /// </summary>
        ConnectionRefusedECONNREFUSED,
        /// <summary>
        /// Host is down (EHOSTDOWN).
        /// </summary>
        HostIsDownEHOSTDOWN,
        /// <summary>
        /// No route to host (EHOSTUNREACH).
        /// </summary>
        NoRouteToHostEHOSTUNREACH,
        /// <summary>
        /// Operation already in progress (EALREADY).
        /// </summary>
        OperationAlreadyInProgressEALREADY,
        /// <summary>
        /// Operation now in progress (EINPROGRESS).
        /// </summary>
        OperationNowInProgressEINPROGRESS,
        /// <summary>
        /// Stale file handle (ESTALE).
        /// </summary>
        StaleFileHandleESTALE,
        /// <summary>
        /// Structure needs cleaning (EUCLEAN).
        /// </summary>
        StructureNeedsCleaningEUCLEAN,
        /// <summary>
        /// Not a XENIX named type file (ENOTNAM).
        /// </summary>
        NotAXENIXNamedTypeFileENOTNAM,
        /// <summary>
        /// No XENIX semaphores available (ENAVAIL).
        /// </summary>
        NoXENIXSemaphoresAvailableENAVAIL,
        /// <summary>
        /// Is a named type file (EISNAM).
        /// </summary>
        IsANamedTypeFileEISNAM,
        /// <summary>
        /// Remote I/O error (EREMOTEIO).
        /// </summary>
        RemoteIOErrorEREMOTEIO,
        /// <summary>
        /// Disk quota exceeded (EDQUOT).
        /// </summary>
        DiskQuotaExceededEDQUOT,
        /// <summary>
        /// No medium found (ENOMEDIUM).
        /// </summary>
        NoMediumFoundENOMEDIUM,
        /// <summary>
        /// Wrong medium type (EMEDIUMTYPE).
        /// </summary>
        WrongMediumTypeEMEDIUMTYPE,
        /// <summary>
        /// Operation canceled (ECANCELED).
        /// </summary>
        OperationCanceledECANCELED,
        /// <summary>
        /// Required key not available (ENOKEY).
        /// </summary>
        RequiredKeyNotAvailableENOKEY,
        /// <summary>
        /// Key has expired (EKEYEXPIRED).
        /// </summary>
        KeyHasExpiredEKEYEXPIRED,
        /// <summary>
        /// Key has been revoked (EKEYREVOKED).
        /// </summary>
        KeyHasBeenRevokedEKEYREVOKED,
        /// <summary>
        /// Key was rejected by service (EKEYREJECTED).
        /// </summary>
        KeyWasRejectedByServiceEKEYREJECTED,
        /// <summary>
        /// Owner died (EOWNERDEAD).
        /// </summary>
        OwnerDiedEOWNERDEAD,
        /// <summary>
        /// State not recoverable (ENOTRECOVERABLE).
        /// </summary>
        StateNotRecoverableENOTRECOVERABLE,
        /// <summary>
        /// Operation not possible due to RF-kill (ERFKILL).
        /// </summary>
        OperationNotPossibleDueToRFKillERFKILL,
        /// <summary>
        /// Memory page has hardware error (EHWPOISON).
        /// </summary>
        MemoryPageHasHardwareErrorEHWPOISON,
    };

    /// <summary>
    /// Contains the available terminal font weights.
    /// </summary>
    enum class FontWeight
    {
        /// <summary>
        /// Usually rendered as bold weight and/or with bright colors.
        /// </summary>
        Bold = 1,
        /// <summary>
        /// Usually rendered with faded colors.
        /// </summary>
        Light
    };

    /// <summary>
    /// Contains the available terminal font layers.
    /// </summary>
    enum class FontLayer
    {
        /// <summary>
        /// Refers to the graphemes.
        /// </summary>
        Foreground = 3,
        /// <summary>
        /// Refers to the background of the graphemes.
        /// </summary>
        Background
    };

    /// <summary>
    /// Contains the available terminal effects.
    /// </summary>
    enum class FontEffect
    {
        /// <summary>
        /// Makes the text curly.
        /// </summary>
        Italic = 1 << 3,
        /// <summary>
        /// Draws a horizontal line crossing below the text.
        /// </summary>
        Underline = 1 << 4,
        /// <summary>
        /// Makes the text blink indefinitely.
        /// </summary>
        Blink = 1 << 5,
        /// <summary>
        /// Swaps the background and foreground colors.
        /// </summary>
        Negative = 1 << 7,
        /// <summary>
        /// Makes the text hard to see or invisible.
        /// </summary>
        Hidden = 1 << 8,
        /// <summary>
        /// Draws a horizontal line crossing through the middle of the text.
        /// </summary>
        Strike = 1 << 9
    };

    /// <summary>
    /// Contains the ANSI codes of the first 16 colors of the XTerm color palette.
    /// </summary>
    enum class XColor
    {
        /// <summary>
        /// The dark variant of the black color.
        /// </summary>
        DarkBlack,
        /// <summary>
        /// The dark variant of the red color.
        /// </summary>
        DarkRed,
        /// <summary>
        /// The dark variant of the green color.
        /// </summary>
        DarkGreen,
        /// <summary>
        /// The dark variant of the yellow color.
        /// </summary>
        DarkYellow,
        /// <summary>
        /// The dark variant of the blue color.
        /// </summary>
        DarkBlue,
        /// <summary>
        /// The dark variant of the magenta color.
        /// </summary>
        DarkMagenta,
        /// <summary>
        /// The dark variant of the cyan color.
        /// </summary>
        DarkCyan,
        /// <summary>
        /// The dark variant of the white color.
        /// </summary>
        DarkWhite,
        /// <summary>
        /// The light variant of the black color.
        /// </summary>
        LightBlack,
        /// <summary>
        /// The light variant of the red color.
        /// </summary>
        LightRed,
        /// <summary>
        /// The light variant of the green color.
        /// </summary>
        LightGreen,
        /// <summary>
        /// The light variant of the yellow color.
        /// </summary>
        LightYellow,
        /// <summary>
        /// The light variant of the blue color.
        /// </summary>
        LightBlue,
        /// <summary>
        /// The light variant of the magenta color.
        /// </summary>
        LightMagenta,
        /// <summary>
        /// The light variant of the cyan color.
        /// </summary>
        LightCyan,
        /// <summary>
        /// The light variant of the white color.
        /// </summary>
        LightWhite
    };

    /// <summary>
    /// Contains the available terminal cursor shapes.
    /// </summary>
    enum class CursorShape
    {
        /// <summary>
        /// Fills the whole character cell.
        /// </summary>
        Block = 2,
        /// <summary>
        /// Fills a region at the bottom of the character cell.
        /// </summary>
        Underline = 4,
        /// <summary>
        /// Fills a region at the left of the character cell.
        /// </summary>
        Bar = 6
    };

    /// <summary>
    /// Contains the available terminal event types.
    /// </summary>
    enum class EventType
    {
        /// <summary>
        /// No event is available.
        /// </summary>
        None,
        /// <summary>
        /// The event timer ran out.
        /// </summary>
        TimeOut,
        /// <summary>
        /// The terminal window was resized.
        /// </summary>
        Resize,
        /// <summary>
        /// The terminal window gained/lost focus.
        /// </summary>
        Focus,
        /// <summary>
        /// The mouse was moved or one of its buttons was clicked.
        /// </summary>
        Mouse,
        /// <summary>
        /// A keybord key was pressed.
        /// </summary>
        Key
    };

    /// <summary>
    /// Contains the available mouse buttons.
    /// </summary>
    enum class MouseButton
    {
        /// <summary>
        /// No button.
        /// </summary>
        None,
        /// <summary>
        /// The left mouse button.
        /// </summary>
        Left,
        /// <summary>
        /// The mouse wheel button.
        /// </summary>
        Wheel,
        /// <summary>
        /// Mouse wheel scroll upwards.
        /// </summary>
        WheelUp,
        /// <summary>
        /// Mouse wheel scroll downwards.
        /// </summary>
        WheelDown,
        /// <summary>
        /// The right mouse button.
        /// </summary>
        Right
    };

    /// <summary>
    /// Contains the available keyboard keys.
    /// </summary>
    enum class KeyboardKey
    {
#ifdef _WIN32
        /// <summary>
        /// The left arrow key.
        /// </summary>
        LeftArrow = -23,
        /// <summary>
        /// The up arrow key.
        /// </summary>
        UpArrow,
        /// <summary>
        /// The right arrow key.
        /// </summary>
        RightArrow,
        /// <summary>
        /// The down arrow key.
        /// </summary>
        DownArrow,
#else
        /// <summary>
        /// The up arrow key.
        /// </summary>
        UpArrow = -23,
        /// <summary>
        /// The down arrow key.
        /// </summary>
        DownArrow,
        /// <summary>
        /// The right arrow key.
        /// </summary>
        RightArrow,
        /// <summary>
        /// The left arrow key.
        /// </summary>
        LeftArrow,
#endif
        /// <summary>
        /// The F1 key.
        /// </summary>
        F1,
        /// <summary>
        /// The F2 key.
        /// </summary>
        F2,
        /// <summary>
        /// The F3 key.
        /// </summary>
        F3,
        /// <summary>
        /// The F4 key.
        /// </summary>
        F4,
        /// <summary>
        /// The F5 key.
        /// </summary>
        F5,
        /// <summary>
        /// The F6 key.
        /// </summary>
        F6,
        /// <summary>
        /// The F7 key.
        /// </summary>
        F7,
        /// <summary>
        /// The F8 key.
        /// </summary>
        F8,
        /// <summary>
        /// The F9 key.
        /// </summary>
        F9,
        /// <summary>
        /// The F10 key.
        /// </summary>
        F10,
        /// <summary>
        /// The F11 key.
        /// </summary>
        F11,
        /// <summary>
        /// The F12 key.
        /// </summary>
        F12,
#ifdef _WIN32
        /// <summary>
        /// The PageUp key.
        /// </summary>
        PageUp,
        /// <summary>
        /// The PageDown key.
        /// </summary>
        PageDown,
        /// <summary>
        /// The End key.
        /// </summary>
        End,
        /// <summary>
        /// The Home key.
        /// </summary>
        Home,
        /// <summary>
        /// The Insert key.
        /// </summary>
        Insert,
        /// <summary>
        /// The Delete key.
        /// </summary>
        Delete,
#else
        /// <summary>
        /// The Home key.
        /// </summary>
        Home,
        /// <summary>
        /// The Insert key.
        /// </summary>
        Insert,
        /// <summary>
        /// The Delete key.
        /// </summary>
        Delete,
        /// <summary>
        /// The End key.
        /// </summary>
        End,
        /// <summary>
        /// The PageUp key.
        /// </summary>
        PageUp,
        /// <summary>
        /// The PageDown key.
        /// </summary>
        PageDown,
#endif
        /// <summary>
        /// The Tab key.
        /// </summary>
        Tab = 9,
#if defined(_WIN32) || defined(__APPLE__)
        /// <summary>
        /// The Enter key.
        /// </summary>
        Enter = 13,
#else
        /// <summary>
        /// The Enter key.
        /// </summary>
        Enter,
#endif
        /// <summary>
        /// The Escape key.
        /// </summary>
        Escape = 27,
        /// <summary>
        /// The space bar key.
        /// </summary>
        SpaceBar = 32,
        /// <summary>
        /// The Backspace key.
        /// </summary>
        Backspace = 127
    };

    class RGBColor;

#ifdef _WIN32
    /// <summary>
    /// Represents an exception thrown whenever an invalid code page tries to be set.
    /// </summary>
    class InvalidCodePageException final
    {
    };
#endif

    /// <summary>
    /// Represents an exception thrown whenever invalid stream attributes tries to be set.
    /// </summary>
    class InvalidStreamAttributesException final
    {
    };

    /// <summary>
    /// Represents an exception thrown whenever a group of streams are wide character oriented.
    /// </summary>
    class WideCharacterOrientationException final
    {
    };

    /// <summary>
    /// Represents an exception thrown whenever a group of streams are not TTY.
    /// </summary>
    class NoValidTTYException final
    {
    };

    /// <summary>
    /// Represents an exception thrown whenever a value is out of a certain range.
    /// </summary>
    class OutOfRangeException final
    {
    };

    /// <summary>
    /// Represents an exception thrown whenever an invalid event type data tries to be read.
    /// </summary>
    class InvalidEventTypeException final
    {
    };

    /// <summary>
    /// Represents the command line arguments.
    /// </summary>
    class CMDArguments final
    {
    public:
#ifdef _WIN32
        /// <summary>
        /// Creates an instance of the CMDArguments class.
        /// </summary>
        /// <param name="totalArguments">The total arguments.</param>
        /// <param name="utf16Arguments">The arguments in UTF-16 encoding.</param>
        /// <param name="utf8Arguments">The arguments in UTF-8 encoding.</param>
        CMDArguments(int totalArguments, wchar_t** utf16Arguments, char** utf8Arguments);
        /// <summary>
        /// Destroys an instance of the CMDArguments class.
        /// </summary>
        ~CMDArguments();
        /// <summary>
        /// Gets an argument in UTF-16 encoding by using its offset.
        /// </summary>
        /// <param name="offset">The offset being referred to.</param>
        /// <returns>The argument in UTF-16 encoding.</returns>
        /// <exception cref="OutOfRangeException">Thrown when the offset is out of the valid range of arguments.</exception>
        std::wstring GetUTF16ArgumentByOffset(std::size_t offset) const;
#else
        /// <summary>
        /// Creates an instance of the CMDArguments class.
        /// </summary>
        /// <param name="totalArguments">The total arguments</param>
        /// <param name="utf8Arguments">The arguments in UTF-8 encoding</param>
        CMDArguments(int totalArguments, char** utf8Arguments);
#endif
        /// <summary>
        /// Gets the total arguments.
        /// </summary>
        /// <returns>The total arguments.</returns>
        int GetTotalArguments() const;
        /// <summary>
        /// Gets an argument in UTF-8 encoding by using its offset.
        /// </summary>
        /// <param name="offset">The offset being referred to.</param>
        /// <returns>The argument in UTF-8 encoding.</returns>
        /// <exception cref="OutOfRangeException">Thrown when the offset is out of the valid range of arguments.</exception>
        std::string GetUTF8ArgumentByOffset(std::size_t offset) const;

    private:
#ifdef _WIN32
        /// <summary>
        /// The arguments in UTF-16 encoding.
        /// </summary>
        wchar_t** m_utf16Arguments;
#endif
        /// <summary>
        /// The arguments in UTF-8 encoding.
        /// </summary>
        char** m_utf8Arguments;
        /// <summary>
        /// The total arguments.
        /// </summary>
        int m_totalArguments;
    };

    /// <summary>
    /// Represents a coordinate inside of the terminal window.
    /// </summary>
    class Coordinate final
    {
    public:
        /// <summary>
        /// Creates an instance of the Coordinate class.
        /// </summary>
        Coordinate();
        /// <summary>
        /// Creates an instance of the Coordinate class.
        /// </summary>
        /// <param name="column">The column component of the coordinate.</param>
        /// <param name="row">The row component of the coordinate.</param>
        Coordinate(unsigned short column, unsigned short row);
        /// <summary>
        /// Gets the column component of the coordinate.
        /// </summary>
        /// <returns>The column component of the coordinate.</returns>
        unsigned short GetColumn() const;
        /// <summary>
        /// Gets the rows component of the coordinate.
        /// </summary>
        /// <returns>The row component of the coordinate.</returns>
        unsigned short GetRow() const;
        /// <summary>
        /// Sets the column component of the coordinate.
        /// </summary>
        /// <param name="column">The column component to be set.</param>
        void SetColumn(unsigned short column);
        /// <summary>
        /// Sets the row component of the coordinate.
        /// </summary>
        /// <param name="row">The row component to be set.</param>
        void SetRow(unsigned short row);
        /// <summary>
        /// Gets a string representation of the coordinate.
        /// </summary>
        /// <returns>A string representation of the coordinate.</returns>
        std::string ToString() const;

    private:
        /// <summary>
        /// The column component of the coordinate.
        /// </summary>
        unsigned short m_column;
        /// <summary>
        /// The row component of the coordinate.
        /// </summary>
        unsigned short m_row;
    };

    /// <summary>
    /// Represents a region inside of the terminal window.
    /// </summary>
    class Region final
    {
    public:
        /// <summary>
        /// Creates a region of the Region class.
        /// </summary>
        Region();
        /// <summary>
        /// Creates an instance of the Region class.
        /// </summary>
        /// <param name="totalColumns">The total columns of the region.</param>
        /// <param name="totalRows">The total rows of the region.</param>
        Region(unsigned short totalColumns, unsigned short totalRows);
        /// <summary>
        /// Creates an instance of the Region class.
        /// </summary>
        /// <param name="coordinateI">The first coordinate of the region.</param>
        /// <param name="coordinateII">The second coordinate of the region. It must be opposite to the first one.</param>
        Region(Coordinate coordinateI, Coordinate coordinateII);
        /// <summary>
        /// Gets the total columns of the region.
        /// </summary>
        /// <returns>The total columns of the region.</returns>
        unsigned short GetTotalColumns() const;
        /// <summary>
        /// Gets the total rows of the region.
        /// </summary>
        /// <returns>The total rows of the region.</returns>
        unsigned short GetTotalRows() const;
        /// <summary>
        /// Gets the area of the region.
        /// </summary>
        /// <returns>The area of the region.</returns>
        unsigned int GetArea() const;
        /// <summary>
        /// Gets the top left coordinate of the region.
        /// </summary>
        /// <returns>The top left coordinate of the region.</returns>
        Coordinate GetTopLeftCoordinate() const;
        /// <summary>
        /// Gets the top right coordinate of the region.
        /// </summary>
        /// <returns>The top right coordinate of the region.</returns>
        Coordinate GetTopRightCoordinate() const;
        /// <summary>
        /// Gets the bottom left coordinate of the region.
        /// </summary>
        /// <returns>The bottom left coordinate of the region.</returns>
        Coordinate GetBottomLeftCoordinate() const;
        /// <summary>
        /// Gets the bottom right coordinate of the region.
        /// </summary>
        /// <returns>The bottom right coordinate of the region.</returns>
        Coordinate GetBottomRightCoordinate() const;
        /// <summary>
        /// Checks if the region contains a coordinate.
        /// </summary>
        /// <param name="column">The column component of the coordinate.</param>
        /// <param name="row">The row component of the coordinate.</param>
        /// <returns></returns>
        bool Contains(unsigned short column, unsigned short row) const;
        /// <summary>
        /// Checks if the region contains a coordinate.
        /// </summary>
        /// <param name="coordinate">The coordinate to be checked.</param>
        /// <returns>A boolean that states the region contains the coordinate.</returns>
        bool Contains(Coordinate coordinate) const;

    private:
        /// <summary>
        /// The total columns of the region.
        /// </summary>
        unsigned short m_totalColumns;
        /// <summary>
        /// The total rows of the region.
        /// </summary>
        unsigned short m_totalRows;
        /// <summary>
        /// The area of the region.
        /// </summary>
        unsigned int m_area;
        /// <summary>
        /// The top left coordinate of the region.
        /// </summary>
        Coordinate m_topLeftCoordinate;
        /// <summary>
        /// The top right coordinate of the region.
        /// </summary>
        Coordinate m_topRightCoordinate;
        /// <summary>
        /// The bottom left coordinate of the region.
        /// </summary>
        Coordinate m_bottomLeftCoordinate;
        /// <summary>
        /// The bottom right coordinate of the region.
        /// </summary>
        Coordinate m_bottomRightCoordinate;
    };

    /// <summary>
    /// Represents a color in hex format.
    /// </summary>
    class HexColor final
    {
    public:
        /// <summary>
        /// Creates an instance of the HexColor class.
        /// </summary>
        /// <param name="code">The hex code of the color. It must be a value in range from 0x0 to 0xffffff.</param>
        /// <exception cref="OutOfRangeException">Thrown when the hex code is out of the valid range.</exception>
        HexColor(unsigned int code);
        /// <summary>
        /// Creates an instance of the HexColor class.
        /// </summary>
        /// <param name="color">A color in RGB format to be converted.</param>
        HexColor(RGBColor color);
        /// <summary>
        /// Gets the hex code of the color.
        /// </summary>
        /// <returns>The hex code of the color.</returns>
        unsigned int GetCode() const;
        /// <summary>
        /// Sets the hex code of the color.
        /// </summary>
        /// <param name="code">The hex code to be set.</param>
        /// <exception cref="OutOfRangeException">Thrown when the hex code is out of the valid range.</exception>
        void SetCode(unsigned int code);
        /// <summary>
        /// Gets a string representation of the color.
        /// </summary>
        /// <param name="hasPrefix">A boolean that states the string has the 0x prefix.</param>
        /// <param name="hasZeroPadding">A boolean that states the string has a left padding filled by the number zero.</param>
        /// <param name="isUpperCase">A boolean that states the string is upper case.</param>
        /// <returns>A string representation of the color.</returns>
        std::string ToString(bool hasPrefix, bool hasZeroPadding, bool isUpperCase) const;

    private:
        /// <summary>
        /// The hex code of the color.
        /// </summary>
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
        std::string ToString() const;

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
