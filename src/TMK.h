#pragma region Headers
#include <format>
#include <iostream>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#pragma endregion

/// <summary>
/// Terminal Manipulation Kit (TMK) - An open-source C++ terminal manipulation library for Windows and Linux that includes features to handle terminal attributes, arguments and
/// events targetting UTF-8 encoding.
/// </summary>
namespace TMK
{
#pragma region Enums
    /// <summary>
    /// Contains the Linux exit codes.
    /// </summary>
    enum class ExitCode
    {
        /// <summary>
        /// Successful execution: thrown when the program execution succeeds.
        /// </summary>
        Success,
        /// <summary>
        /// Generic failed execution: thrown when the program execution failed without further description.
        /// </summary>
        Failure,
        /// <summary>
        /// Operation not permitted (EPERM): thrown when the current user does not have enough privileges to complete an operation.
        /// </summary>
        OperationNotPermittedEPERM = 1,
        /// <summary>
        /// No such file or directory (ENOENT): thrown when a non-existent file system entry is referenced.
        /// </summary>
        NoSuchFileOrDirectoryENOENT,
        /// <summary>
        /// No such process (ESRCH): thrown when a non-existent process is referenced.
        /// </summary>
        NoSuchProcessESRCH,
        /// <summary>
        /// Interrupted system call (EINTR): thrown when a blocking system call is interrupted by a signal handler.
        /// </summary>
        InterruptedSystemCallEINTR,
        /// <summary>
        /// Input or output error (EIO): thrown when an input or output operation fails.
        /// </summary>
        InputOutputErrorEIO,
        /// <summary>
        /// No such device or address (ENXIO): thrown when an invalid device or device address is referenced.
        /// </summary>
        NoSuchDeviceOrAddressENXIO,
        /// <summary>
        /// Argument list too long (E2BIG): thrown when the total arguments given to a process exceeds the system limit.
        /// </summary>
        ArgumentListTooLongE2BIG,
        /// <summary>
        /// Exec format error (ENOEXEC): thrown when a binary can not be executed.
        /// </summary>
        ExecFormatErrorENOEXEC,
        /// <summary>
        /// Bad file descriptor (EBADF): thrown when an invalid file descriptor is referenced.
        /// </summary>
        BadFileDescriptorEBADF,
        /// <summary>
        /// No child processes (ECHILD): thrown when a group of processes referenced in a wait call does not exist or is not owned by the calling process.
        /// </summary>
        NoChildProcessesECHILD,
        /// <summary>
        /// Resource temporarily unavailable (EAGAIN): thrown when a hardware or software resource is currently unavailable or can not be completed.
        /// </summary>
        ResourceTemporarilyUnavailableEAGAIN,
        /// <summary>
        /// Resource temporarily unavailable (EWOULDBLOCK): thrown when a hardware or software resource is currently unavailable or can not be completed.
        /// </summary>
        ResourceTemporarilyUnavailableEWOULDBLOCK = 11,
        /// <summary>
        /// Can not allocate memory (ENOMEM): thrown when the system does not have more memory available.
        /// </summary>
        CanNotAllocateMemoryENOMEM,
        /// <summary>
        /// Permission denied (EACCES): thrown when the current user does not have enough privileges to complete a file system operation.
        /// </summary>
        PermissionDeniedEACCES,
        /// <summary>
        /// Bad address (EFAULT): thrown when a process attemps to access a not owned memory address.
        /// </summary>
        BadAddressEFAULT,
        /// <summary>
        /// Block device required (ENOTBLK): thrown when an invalid resource is reference in place of a block device.
        /// </summary>
        BlockDeviceRequiredENOTBLK,
        /// <summary>
        /// Device or resource busy (EBUSY): thrown when a resource is currently in use by a process.
        /// </summary>
        DeviceOrResourceBusyEBUSY,
        /// <summary>
        /// File exists (EEXIST): thrown when a file system entry already exists.
        /// </summary>
        FileExistsEEXIST,
        /// <summary>
        /// Invalid cross-device link (EXDEV): thrown when a file transfer operating between two file systems fails.
        /// </summary>
        InvalidCrossDeviceLinkEXDEV,
        /// <summary>
        /// No such device (ENODEV): thrown when a non-existent device is referenced.
        /// </summary>
        NoSuchDeviceENODEV,
        /// <summary>
        /// Not a directory (ENOTDIR): thrown when an invalid file system entry is referenced in place of a directory.
        /// </summary>
        NotADirectoryENOTDIR,
        /// <summary>
        /// Is a directory (EISDIR): thrown when an invalid operation is attemped on a directory.
        /// </summary>
        IsADirectoryEISDIR,
        /// <summary>
        /// Invalid argument (EINVAL): thrown when an operation is attemped with invalid arguments.
        /// </summary>
        InvalidArgumentEINVAL,
        /// <summary>
        /// Too many open files in system (ENFILE): thrown when the total of open file descriptors has exceeded the system-wide limit.
        /// </summary>
        TooManyOpenFilesInSystemENFILE,
        /// <summary>
        /// Too many open files (EMFILE): thrown when the total of open file descriptors has exceeded the process limit.
        /// </summary>
        TooManyOpenFilesEMFILE,
        /// <summary>
        /// Inappropriate ioctl for device (ENOTTY): thrown when an input or output operation is attemped on a device that does not support it.
        /// </summary>
        InappropriateIoctlForDeviceENOTTY,
        /// <summary>
        /// Text file busy (ETXTBSY): thrown when a file descriptor is in use by a process.
        /// </summary>
        TextFileBusyETXTBSY,
        /// <summary>
        /// File too large (EFBIG): thrown when a write operation fails due to the system file size limit has been reached.
        /// </summary>
        FileTooLargeEFBIG,
        /// <summary>
        /// No space left on device (ENOSPC): thrown when a device does not have space available.
        /// </summary>
        NoSpaceLeftOnDeviceENOSPC,
        /// <summary>
        /// Illegal seek (ESPIPE): thrown when a seek operation is attemped on a file descriptor that does not supports it.
        /// </summary>
        IllegalSeekESPIPE,
        /// <summary>
        /// Read-only file system (EROFS): thrown when a write operation is attemped on a read-only file system.
        /// </summary>
        ReadOnlyFileSystemEROFS,
        /// <summary>
        /// Too many links (EMLINK): thrown when a file reaches the system limit of hard links.
        /// </summary>
        TooManyLinksEMLINK,
        /// <summary>
        /// Broken pipe (EPIPE): thrown when a write operation is attemped on a closed pipe.
        /// </summary>
        BrokenPipeEPIPE,
        /// <summary>
        /// Numerical argument out of domain (EDOM): thrown when an argument of invalid domain is used in a math operation.
        /// </summary>
        NumericalArgumentOutOfDomainEDOM,
        /// <summary>
        /// Numerical result out of range (ERANGE): thrown when a value can not be represented within a given range.
        /// </summary>
        NumericalResultOutOfRangeERANGE,
        /// <summary>
        /// Resource deadlock avoided (EDEADLK): thrown when two processes depend on resources of each other to complete an operation.
        /// </summary>
        ResourceDeadlockAvoidedEDEADLK,
        /// <summary>
        /// Resource deadlock avoided (EDEADLOCK): thrown when two processes depend on resources of each other to complete an operation.
        /// </summary>
        ResourceDeadlockAvoidedEDEADLOCK = 35,
        /// <summary>
        /// File name too long (ENAMETOOLONG): thrown when a file system entry has a name that exceeds the system limit.
        /// </summary>
        FileNameTooLongENAMETOOLONG,
        /// <summary>
        /// No locks available (ENOLCK): thrown when there are no NFS locks available.
        /// </summary>
        NoLocksAvailableENOLCK,
        /// <summary>
        /// Function not implemented (ENOSYS): thrown when a non-existent system call is referenced.
        /// </summary>
        FunctionNotImplementedENOSYS,
        /// <summary>
        /// Directory not empty (ENOTEMPTY): thrown an operation can not be performed on a non-empty directory.
        /// </summary>
        DirectoryNotEmptyENOTEMPTY,
        /// <summary>
        /// Too many levels of symbolic links (ELOOP): thrown when an operation can not be performed due to recursive calls on the same symbolic link.
        /// </summary>
        TooManyLevelsOfSymbolicLinksELOOP,
        /// <summary>
        /// No message of desired type (ENOMSG): thrown when a message queue does match the desired type.
        /// </summary>
        NoMessageOfDesiredTypeENOMSG = 42,
        /// <summary>
        /// Identifier removed (EIDRM): thrown when an inter-process communication resource is removed during an operation.
        /// </summary>
        IdentifierRemovedEIDRM,
        /// <summary>
        /// Channel number out of range (ECHRNG): thrown when an invalid device channel number is referenced.
        /// </summary>
        ChannelNumberOutOfRangeECHRNG,
        /// <summary>
        /// Level 2 not synchronized (EL2NSYNC): thrown when a netlink operation fails due to a synchronization issue.
        /// </summary>
        Level2NotSynchronizedEL2NSYNC,
        /// <summary>
        /// Level 3 halted (EL3HLT): thrown when a required hardware state is not available.
        /// </summary>
        Level3HaltedEL3HLT,
        /// <summary>
        /// Level 3 reset (EL3RST): thrown when a device or subsystem reset fails.
        /// </summary>
        Level3ResetEL3RST,
        /// <summary>
        /// Link number out of range (ELNRNG): thrown when an invalid hard or symbolic link is referenced.
        /// </summary>
        LinkNumberOutOfRangeELNRNG,
        /// <summary>
        /// Protocol driver not attached (EUNATCH): thrown when a terminal device is not attached to a protocol driver.
        /// </summary>
        ProtocolDriverNotAttachedEUNATCH,
        /// <summary>
        /// No CSI structure available (ENOCSI): thrown when a terminal CSI structure is not available or valid.
        /// </summary>
        NoCSIStructureAvailableENOCSI,
        /// <summary>
        /// Level 2 halted (EL2HLT): thrown when the requested network protocol is unavailable in the system.
        /// </summary>
        Level2HaltedEL2HLT,
        /// <summary>
        /// Invalid exchange (EBADE): thrown when an invalid exchange is specified to a system call.
        /// </summary>
        InvalidExchangeEBADE,
        /// <summary>
        /// Invalid request descriptor (EBADR): thrown when an invalid request descriptor is specified to a system call.
        /// </summary>
        InvalidRequestDescriptorEBADR,
        /// <summary>
        /// Exchange full (EXFULL): thrown when a communication exchange queue is full.
        /// </summary>
        ExchangeFullEXFULL,
        /// <summary>
        /// No anode (ENOANO): thrown when a non-existent anode is referenced.
        /// </summary>
        NoAnodeENOANO,
        /// <summary>
        /// Invalid request code (EBADRQC): thrown when an invalid system call request code is used.
        /// </summary>
        InvalidRequestCodeEBADRQC,
        /// <summary>
        /// Invalid slot (EBADSLT): thrown when an invalid slot number is specified in a system call.
        /// </summary>
        InvalidSlotEBADSLT,
        /// <summary>
        /// Bad font file format (EBFONT): thrown when an font file is invalid or corrupted.
        /// </summary>
        BadFontFileFormatEBFONT = 59,
        /// <summary>
        /// Device not a stream (ENOSTR): thrown when a file descriptor referenced does not support streaming operations.
        /// </summary>
        DeviceNotAStreamENOSTR,
        /// <summary>
        /// No data available (ENODATA): thrown when no data is available during a non-blocking read operation.
        /// </summary>
        NoDataAvailableENODATA,
        /// <summary>
        /// Timer expired (ETIME): thrown when a timer associated to an operation expires before it has been completed.
        /// </summary>
        TimerExpiredETIME,
        /// <summary>
        /// Out of streams resources (ENOSR): thrown when the system can not allocate resources for streaming operations.
        /// </summary>
        OutOfStreamsResourcesENOSR,
        /// <summary>
        /// Machine is not on the network (ENONET): thrown when an operation fails due to the machine not being connected to the required network.
        /// </summary>
        MachineIsNotOnTheNetworkENONET,
        /// <summary>
        /// Package not installed (ENOPKG): thrown when a package or component is not installed in the system.
        /// </summary>
        PackageNotInstalledENOPKG,
        /// <summary>
        /// Object is remote (EREMOTE): thrown when an object referred is on a remote system.
        /// </summary>
        ObjectIsRemoteEREMOTE,
        /// <summary>
        /// Link has been severed (ENOLINK): thrown when the connection to a resource has been closed.
        /// </summary>
        LinkHasBeenSeveredENOLINK,
        /// <summary>
        /// Advertise error (EADV): thrown when a connection advertisement fails.
        /// </summary>
        AdvertiseErrorEADV,
        /// <summary>
        /// Srmount error (ESRMNT): thrown when mouting a device using the srmount feature fails.
        /// </summary>
        SrmountErrorESRMNT,
        /// <summary>
        /// Communication error on send (ECOMM): thrown when communication fails during a send operation.
        /// </summary>
        CommunicationErrorOnSendECOMM,
        /// <summary>
        /// Protocol error (EPROTO): thrown when there is an error at the network protocol.
        /// </summary>
        ProtocolErrorEPROTO,
        /// <summary>
        /// Multihop attempted (EMULTIHOP): thrown when an operation attemps routing hopping without permissions or support.
        /// </summary>
        MultihopAttemptedEMULTIHOP,
        /// <summary>
        /// RFS specific error (EDOTDOT): thrown when there is an error in the RFS system.
        /// </summary>
        RFSSpecificErrorEDOTDOT,
        /// <summary>
        /// Bad message (EBADMSG): thrown when a message received through a connection does not match its protocol.
        /// </summary>
        BadMessageEBADMSG,
        /// <summary>
        /// Value too large for defined data type (EOVERFLOW): thrown when a value can not be hold by its data type.
        /// </summary>
        ValueTooLargeForDefinedDataTypeEOVERFLOW,
        /// <summary>
        /// Name not unique on network (ENOTUNIQ): thrown when the name provided to perform a network operation is not unique.
        /// </summary>
        NameNotUniqueOnNetworkENOTUNIQ,
        /// <summary>
        /// File descriptor in bad state (EBADFD): thrown when an operation is attemped on an invalid file descriptor.
        /// </summary>
        FileDescriptorInBadStateEBADFD,
        /// <summary>
        /// Remote address changed (EREMCHG): thrown when a remote address is changed during connection.
        /// </summary>
        RemoteAddressChangedEREMCHG,
        /// <summary>
        /// Can not access a needed shared library (ELIBACC): thrown when a shared library can not be found or accessed.
        /// </summary>
        CanNotAccessANeededSharedLibraryELIBACC,
        /// <summary>
        /// Accessing a corrupted shared library (ELIBBAD): thrown when a shared library is invalid or corrupted.
        /// </summary>
        AccessingACorruptedSharedLibraryELIBBAD,
        /// <summary>
        /// .lib section in a.out corrupted (ELIBSCN): thrown when the .lib assembly section of an executable is invalid or corrupted.
        /// </summary>
        LibSectionInAOutCorruptedELIBSCN,
        /// <summary>
        /// Attempting to link in too many shared libraries (ELIBMAX): thrown when the total of shared libraries being linked into an executable exceeds the system limit.
        /// </summary>
        AttemptingToLinkInTooManySharedLibrariesELIBMAX,
        /// <summary>
        /// Cannot exec a shared library directly (ELIBEXEC): thrown when a shared library is attemped to be executed directly.
        /// </summary>
        CannotExecASharedLibraryDirectlyELIBEXEC,
        /// <summary>
        /// Invalid or incomplete multibyte or wide character (EILSEQ): thrown when a byte sequence is invalid given the expected encoding.
        /// </summary>
        InvalidOrIncompleteMultibyteOrWideCharacterEILSEQ,
        /// <summary>
        /// Interrupted system call should be restarted (ERESTART): thrown when a system call is interrupted by a signal handler and must be attemped again.
        /// </summary>
        InterruptedSystemCallShouldBeRestartedERESTART,
        /// <summary>
        /// Streams pipe error (ESTRPIPE): thrown when a write operation is perfomed on a closed stream.
        /// </summary>
        StreamsPipeErrorESTRPIPE,
        /// <summary>
        /// Too many users (EUSERS): thrown when the total of network connections or users exceeds the system limit.
        /// </summary>
        TooManyUsersEUSERS,
        /// <summary>
        /// Socket operation on non-socket (ENOTSOCK): thrown when an invalid file system entry is referenced in place of a socket.
        /// </summary>
        SocketOperationOnNonSocketENOTSOCK,
        /// <summary>
        /// Destination address required (EDESTADDRREQ): thrown when no destination address is provided for a network operation.
        /// </summary>
        DestinationAddressRequiredEDESTADDRREQ,
        /// <summary>
        /// Message too long (EMSGSIZE): thrown when the message sent through a socket exceeds the limit predefined by its protocol.
        /// </summary>
        MessageTooLongEMSGSIZE,
        /// <summary>
        /// Protocol wrong type for socket (EPROTOTYPE): thrown when a protocol does match the socket it is attemped on.
        /// </summary>
        ProtocolWrongTypeForSocketEPROTOTYPE,
        /// <summary>
        /// Protocol not available (ENOPROTOOPT): thrown when a protocol requested is not available in the system.
        /// </summary>
        ProtocolNotAvailableENOPROTOOPT,
        /// <summary>
        /// Protocol not supported (EPROTONOSUPPORT): thrown when a protocol is not available in the system.
        /// </summary>
        ProtocolNotSupportedEPROTONOSUPPORT,
        /// <summary>
        /// Socket type not supported (ESOCKTNOSUPPORT): thrown a the socket type is not available in the system.
        /// </summary>
        SocketTypeNotSupportedESOCKTNOSUPPORT,
        /// <summary>
        /// Operation not supported (EOPNOTSUPP): thrown when a socket or its protocol do not support an operation.
        /// </summary>
        OperationNotSupportedEOPNOTSUPP,
        /// <summary>
        /// Operation not supported (ENOTSUP): thrown when a socket or its protocol do not support an operation.
        /// </summary>
        OperationNotSupportedENOTSUP = 95,
        /// <summary>
        /// Protocol family not supported (EPFNOSUPPORT): thrown when a protocol family is not available in the system.
        /// </summary>
        ProtocolFamilyNotSupportedEPFNOSUPPORT,
        /// <summary>
        /// Address family not supported by protocol (EAFNOSUPPORT): thrown when an invalid address family is used in a protocol.
        /// </summary>
        AddressFamilyNotSupportedByProtocolEAFNOSUPPORT,
        /// <summary>
        /// Address already in use (EADDRINUSE): thrown when a socket is attemped to be bind to an address that is already in use.
        /// </summary>
        AddressAlreadyInUseEADDRINUSE,
        /// <summary>
        /// Cannot assign requested address (EADDRNOTAVAIL): thrown when an address can not be bind to a socket due to it is being unavailable.
        /// </summary>
        CannotAssignRequestedAddressEADDRNOTAVAIL,
        /// <summary>
        /// Network is down (ENETDOWN): thrown when the network subsystem is unavailable.
        /// </summary>
        NetworkIsDownENETDOWN,
        /// <summary>
        /// Network is unreachable (ENETUNREACH): thrown when no routing could be found to reach a network.
        /// </summary>
        NetworkIsUnreachableENETUNREACH,
        /// <summary>
        /// Network dropped connection on reset (ENETRESET): thrown when a connection is closed due to a reset.
        /// </summary>
        NetworkDroppedConnectionOnResetENETRESET,
        /// <summary>
        /// Software caused connection abort (ECONNABORTED): thrown when a connection is closed due to a software crash.
        /// </summary>
        SoftwareCausedConnectionAbortECONNABORTED,
        /// <summary>
        /// Connection reset by peer (ECONNRESET): thrown when the connection is forcibly closed by one of its peer.
        /// </summary>
        ConnectionResetByPeerECONNRESET,
        /// <summary>
        /// No buffer space available (ENOBUFS): thrown when the total of network buffers exceeds the system limit.
        /// </summary>
        NoBufferSpaceAvailableENOBUFS,
        /// <summary>
        /// Transport endpoint is already connected (EISCONN): thrown when a connection is attemped on a socket that is already connected.
        /// </summary>
        TransportEndpointIsAlreadyConnectedEISCONN,
        /// <summary>
        /// Transport endpoint is not connected (ENOTCONN): thrown when an operation is attemped on a non-connected socket.
        /// </summary>
        TransportEndpointIsNotConnectedENOTCONN,
        /// <summary>
        /// Can not send after transport endpoint shutdown (ESHUTDOWN): thrown when an operation is attemped on a socket that has been shutdown.
        /// </summary>
        CanNotSendAfterTransportEndpointShutdownESHUTDOWN,
        /// <summary>
        /// Too many references: cannot splice (ETOOMANYREFS): thrown when there are too many references to a kernel resource.
        /// </summary>
        TooManyReferencesCannotSpliceETOOMANYREFSs,
        /// <summary>
        /// Connection timed out (ETIMEDOUT): thrown when a connection can not be stablished in its protocol predefined time.
        /// </summary>
        ConnectionTimedOutETIMEDOUT,
        /// <summary>
        /// Connection refused (ECONNREFUSED): thrown when a connection can not be stablished due to no host been listening to it.
        /// </summary>
        ConnectionRefusedECONNREFUSED,
        /// <summary>
        /// Host is down (EHOSTDOWN): thrown when a connection can not be stablished because the host being reached is down.
        /// </summary>
        HostIsDownEHOSTDOWN,
        /// <summary>
        /// No route to host (EHOSTUNREACH): thrown when no routing could be found to reach a host inside of a network.
        /// </summary>
        NoRouteToHostEHOSTUNREACH,
        /// <summary>
        /// Operation already in progress (EALREADY): thrown when a non-blocking device has another operation currently queue.
        /// </summary>
        OperationAlreadyInProgressEALREADY,
        /// <summary>
        /// Operation now in progress (EINPROGRESS): thrown when a non-blocking device is already running a same operation.
        /// </summary>
        OperationNowInProgressEINPROGRESS,
        /// <summary>
        /// Stale file handle (ESTALE): thrown when a NFS file handle stales and becomes invalid.
        /// </summary>
        StaleFileHandleESTALE,
        /// <summary>
        /// Structure needs cleaning (EUCLEAN): thrown when the file system structure needs cleaning.
        /// </summary>
        StructureNeedsCleaningEUCLEAN,
        /// <summary>
        /// Not a XENIX named type file (ENOTNAM): thrown when an invalid file system entry is referenced in place of a named type file.
        /// </summary>
        NotAXENIXNamedTypeFileENOTNAM,
        /// <summary>
        /// No XENIX semaphores available (ENAVAIL): thrown when the total of semaphores exceeds the system limit.
        /// </summary>
        NoXENIXSemaphoresAvailableENAVAIL,
        /// <summary>
        /// Is a named type file (EISNAM): thrown when a named type file is referenced in place of a non-named type file.
        /// </summary>
        IsANamedTypeFileEISNAM,
        /// <summary>
        /// Remote I/O error (EREMOTEIO): thrown when an input or output error has happened when accessing a remote file system.
        /// </summary>
        RemoteIOErrorEREMOTEIO,
        /// <summary>
        /// Disk quota exceeded (EDQUOT): thrown when a write operation is attemped while the disk quota exceeds its limit.
        /// </summary>
        DiskQuotaExceededEDQUOT,
        /// <summary>
        /// No medium found (ENOMEDIUM): thrown when no medium is found in the drive.
        /// </summary>
        NoMediumFoundENOMEDIUM,
        /// <summary>
        /// Wrong medium type (EMEDIUMTYPE): thrown when the inserted medium is not supported by the drive or it is not formatted.
        /// </summary>
        WrongMediumTypeEMEDIUMTYPE,
        /// <summary>
        /// Operation canceled (ECANCELED): thrown when an asynchronous operation is canceled before it has been completed.
        /// </summary>
        OperationCanceledECANCELED,
        /// <summary>
        /// Required key not available (ENOKEY): thrown when a cryptographic key is not available.
        /// </summary>
        RequiredKeyNotAvailableENOKEY,
        /// <summary>
        /// Key has expired (EKEYEXPIRED): thrown when a cryptographic key has expired.
        /// </summary>
        KeyHasExpiredEKEYEXPIRED,
        /// <summary>
        /// Key has been revoked (EKEYREVOKED): thrown when a cryptographic key has been revoked.
        /// </summary>
        KeyHasBeenRevokedEKEYREVOKED,
        /// <summary>
        /// Key was rejected by service (EKEYREJECTED): thrown when a cryptographic key is rejected by a service due to not match its criteria.
        /// </summary>
        KeyWasRejectedByServiceEKEYREJECTED,
        /// <summary>
        /// Owner died (EOWNERDEAD): thrown when the process owner of a mutex dies without releasing it.
        /// </summary>
        OwnerDiedEOWNERDEAD,
        /// <summary>
        /// State not recoverable (ENOTRECOVERABLE): thrown when the previous state of a mutex can not be restored after it has been unlocked.
        /// </summary>
        StateNotRecoverableENOTRECOVERABLE,
        /// <summary>
        /// Operation not possible due to RF-kill (ERFKILL): thrown when an operation is canceled by the RF-kill feature.
        /// </summary>
        OperationNotPossibleDueToRFKillERFKILL,
        /// <summary>
        /// Memory page has hardware error (EHWPOISON): thrown when a memory page gets corrupted.
        /// </summary>
        MemoryPageHasHardwareErrorEHWPOISON
    };

    /// <summary>
    /// Contains the terminal font layers.
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
    /// Contains the terminal font weights.
    /// </summary>
    enum class FontWeight
    {
        /// <summary>
        /// Usually rendered with bold weight and/or with light colors.
        /// </summary>
        Bold = 1,
        /// <summary>
        /// Usually rendered with faint colors.
        /// </summary>
        Light
    };

    /// <summary>
    /// Contains the first 16 colors of the XTerm color palette.
    /// </summary>
    enum class XTermColor
    {
        /// <summary>
        /// The regular variant of the black color.
        /// </summary>
        Black,
        /// <summary>
        /// The regular variant of the red color.
        /// </summary>
        Red,
        /// <summary>
        /// The regular variant of the green color.
        /// </summary>
        Green,
        /// <summary>
        /// The regular variant of the yellow color.
        /// </summary>
        Yellow,
        /// <summary>
        /// The regular variant of the blue color.
        /// </summary>
        Blue,
        /// <summary>
        /// The regular variant of the magenta color.
        /// </summary>
        Magenta,
        /// <summary>
        /// The regular variant of the cyan color.
        /// </summary>
        Cyan,
        /// <summary>
        /// The regular variant of the white color.
        /// </summary>
        White,
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
#pragma endregion

#pragma region Classes
    /// <summary>
    /// Represents an exception thrown when a group of terminal streams are redirected.
    /// </summary>
    class StreamRedirectionException final
    {
    public:
        /// <summary>
        /// Creates an instance of the StreamRedirectionException class.
        /// </summary>
        StreamRedirectionException() = default;
    };

    /// <summary>
    /// Represents an exception thrown when attributes of a terminal stream are invalid.
    /// </summary>
    class InvalidStreamAttributesException final
    {
    public:
        /// <summary>
        /// Creates an instance of the InvalidStreamAttributesException class.
        /// </summary>
        InvalidStreamAttributesException() = default;
    };

    /// <summary>
    /// Represents an exception thrown when a group of terminal streams are wide character oriented.
    /// </summary>
    class WideCharacterOrientationException final
    {
    public:
        /// <summary>
        /// Creates an instance of the WideCharacterOrientationException class.
        /// </summary>
        WideCharacterOrientationException() = default;
    };

    /// <summary>
    /// Represents an exception thrown when a value is out of a valid range.
    /// </summary>
    class OutOfRangeException final
    {
    public:
        /// <summary>
        /// Creates an instance of the OutOfRangeException class.
        /// </summary>
        OutOfRangeException() = default;
    };

    /// <summary>
    /// Represents the command-line arguments.
    /// </summary>
    class CMDArguments
    {
        friend class Terminal;

    public:
        /// <summary>
        /// Gets the total arguments.
        /// </summary>
        /// <returns>The total arguments.</returns>
        std::size_t GetTotalArguments() noexcept;
        /// <summary>
        /// Gets the UTF-8 encoded arguments.
        /// </summary>
        /// <returns>The UTF-8 encoded arguments.</returns>
        const std::vector<std::string>& GetUTF8Arguments();

    private:
#ifdef _WIN32
        /// <summary>
        /// Creates an instance of the CMDArguments class.
        /// </summary>
        /// <param name="utf16Arguments">The UTF-16 encoded arguments.</param>
        /// <param name="utf8Arguments">The UTF-8 encoded arguments.</param>
        CMDArguments(const std::vector<std::wstring>& utf16Arguments, const std::vector<std::string>& utf8Arguments) noexcept;
        /// <summary>
        /// Gets the UTF-16 encoded arguments.
        /// </summary>
        /// <returns>The UTF-16 encoded arguments.</returns>
        const std::vector<std::wstring>& GetUTF16Arguments();
        /// <summary>
        /// The arguments in UTF-16 encoding.
        /// </summary>
        std::vector<std::wstring> m_utf16Arguments;
#else
        /// <summary>
        /// Creates an instance of the CMDArguments class.
        /// </summary>
        /// <param name="utf8Arguments">The UTF-8 encoded arguments.</param>
        CMDArguments(const std::vector<std::string>& utf8Arguments) noexcept;
#endif
        CMDArguments(const CMDArguments&) = delete;
        /// <summary>
        /// The arguments in UTF-8 encoding.
        /// </summary>
        std::vector<std::string> m_utf8Arguments;
    };

    /// <summary>
    /// Represents the terminal.
    /// </summary>
    class Terminal final
    {
    private:
        /// <summary>
        /// A boolean that states the redirection cache of the terminal streams has been initialized.
        /// </summary>
        static bool s_hasInitializedStreamRedirectionCache;
        /// <summary>
        /// A boolean that states the terminal input stream is redirected.
        /// </summary>
        static bool s_isInputRedirected;
        /// <summary>
        /// A boolean that states the terminal output stream is redirected.
        /// </summary>
        static bool s_isOutputRedirected;
        /// <summary>
        /// A boolean that states the terminal error stream is redirected.
        /// </summary>
        static bool s_isErrorRedirected;

#ifdef _WIN32
        /// <summary>
        /// Initializes the virtual terminal processing: allowing terminals on Windows to process ANSI escape sequences.
        /// </summary>
        static void InitializeVirtualTerminalProcessing() noexcept;
#endif
        /// <summary>
        /// Initializes the redirection cache of the terminal streams.
        /// </summary>
        static void InitializeStreamRedirectionCache() noexcept;
        /// <summary>
        /// Writes an ANSI escape sequence to the terminal output or error streams.
        /// </summary>
        /// <typeparam name="...Args">The type of the arguments to be formatted.</typeparam>
        /// <param name="format">The format to be used. It accepts the same format specifiers as the std::format function family.</param>
        /// <param name="...arguments">The arguments to be formatted.</param>
        template <class... Args>
        static void WriteANSIEscapeSequence(const std::string_view& format, Args... arguments)
        {
            if (!Terminal::Output::IsRedirected())
            {
                Terminal::Output::Write(format, arguments...);
            }
            else if (!Terminal::Error::IsRedirected())
            {
                Terminal::Error::Write(format, arguments...);
            }
        }
        /// <summary>
        /// Resets all terminal attributes.
        /// </summary>
        static void ResetAttributes() noexcept;

        Terminal() = delete;

        /// <summary>
        /// Represents a terminal stream.
        /// </summary>
        /// <typeparam name="N">The file ID related to the stream.</typeparam>
        template <int N>
        class Stream
        {
        public:
#ifdef _WIN32
            /// <summary>
            /// Gets the handle related to the terminal stream.
            /// </summary>
            /// <returns>The handle related to the terminal stream.</returns>
            static HANDLE GetHandle() noexcept
            {
                return GetStdHandle(!N ? STD_INPUT_HANDLE : N == 1 ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
            }
            /// <summary>
            /// Gets the mode of the terminal stream.
            /// </summary>
            /// <returns>The mode of the terminal stream.</returns>
            /// <exception cref="StreamRedirectionException">Thrown whenever the stream is redirected.</exception>
            static DWORD GetMode()
            {
                DWORD mode;
                if (!GetConsoleMode(GetHandle(), &mode))
                {
                    throw StreamRedirectionException();
                }
                return mode;
            }
            /// <summary>
            /// Sets the mode of the terminal stream.
            /// </summary>
            /// <param name="mode">The mode to be set.</param>
            /// <exception cref="StreamRedirectionException">Thrown whenever the stream is redirected.</exception>
            /// <exception cref="InvalidStreamAttributesException">Thrown whenever the mode is invalid.</exception>
            static void SetMode(DWORD mode)
            {
                if (IsRedirected())
                {
                    throw StreamRedirectionException();
                }
                if (!SetConsoleMode(GetHandle(), mode))
                {
                    throw InvalidStreamAttributesException();
                }
            }
#endif
            /// <summary>
            /// Gets the file related to the terminal stream.
            /// </summary>
            /// <returns>The file related to the terminal stream.</returns>
            static std::FILE* GetFile() noexcept
            {
                return !N ? stdin : N == 1 ? stdout : stderr;
            }
            /// <summary>
            /// Gets the file ID related to the terminal stream.
            /// </summary>
            /// <returns>The file ID related to the terminal stream.</returns>
            static int GetFileID() noexcept
            {
                return N;
            }
            /// <summary>
            /// Checks if the terminal stream is redirected.
            /// </summary>
            /// <returns>A boolean that states the terminal stream is redirected.</returns>
            static bool IsRedirected() noexcept
            {
                InitializeStreamRedirectionCache();
                return !N ? s_isInputRedirected : N == 1 ? s_isOutputRedirected : s_isErrorRedirected;
            }

        private:
            Stream() = delete;
        };

        /// <summary>
        /// Represents a terminal stream.
        /// </summary>
        /// <typeparam name="N">The file ID related to the stream.</typeparam>
        template <int N>
        class WritableStream : public Stream<N>
        {
        public:
            /// <summary>
            /// Gets the ostream related to the terminal stream.
            /// </summary>
            /// <returns>The ostream related to the terminal stream.</returns>
            static std::ostream& GetOStream() noexcept
            {
                return N == 1 ? std::cout : std::cerr;
            }
            /// <summary>
            /// Formats and writes a string to the terminal stream.
            /// </summary>
            /// <typeparam name="T">The type of an argument to be formatted.</typeparam>
            /// <param name="argument">An argument to be formatted.</param>
            template <class T>
            static void Write(T argument)
            {
                InitializeStreamRedirectionCache();
                if (N == 2)
                {
                    Output::Flush();
                }
                GetOStream() << argument;
            }
            /// <summary>
            /// Formats and writes a string to the terminal stream.
            /// </summary>
            /// <typeparam name="...Args">The type of the arguments to be formatted.</typeparam>
            /// <param name="format">The format to be used. It uses the same format specifiers as the std::format function family.</param>
            /// <param name="...arguments">The arguments to be formatted.</param>
            template <class... Args>
            static void Write(const std::string_view& format, Args... arguments)
            {
                InitializeStreamRedirectionCache();
                if (N == 2)
                {
                    Output::Flush();
                }
                std::cout << std::vformat(format, std::make_format_args(arguments...));
            }
            /// <summary>
            /// Writes a newline grapheme to the terminal stream.
            /// </summary>
            static void WriteLine()
            {
                Write("\n");
            }
            /// <summary>
            /// Formats and writes a string to the terminal stream with a newline grapheme appended to its end.
            /// </summary>
            /// <typeparam name="T">The type of an argument to be formatted.</typeparam>
            /// <param name="argument">An argument to be formatted.</param>
            template <class T>
            static void WriteLine(T argument)
            {
                Write(argument);
                std::cout << std::endl;
            }
            /// <summary>
            /// Formats and writes a string to the terminal stream with a newline grapheme appended to its end.
            /// </summary>
            /// <typeparam name="...Args">The type of the arguments to be formatted.</typeparam>
            /// <param name="format">The format to be used. It uses the same format specifiers as the std::format function family.</param>
            /// <param name="...arguments">The arguments to be formatted.</param>
            template <class... Args>
            static void WriteLine(const std::string_view& format, Args... arguments)
            {
                Write(format, arguments...);
                std::cout << std::endl;
            }
        };

    public:
#ifdef _WIN32
        /// <summary>
        /// Represents the terminal encoding.
        /// </summary>
        class Encoding final
        {
        public:
            /// <summary>
            /// Sets the code page of the terminal output stream.
            /// </summary>
            /// <param name="codePage">The code page to be set.</param>
            /// <exception cref="InvalidStreamAttributesException">Thrown when the code page is invalid.</exception>
            static void SetOutputCodePage(UINT codePage);
            /// <summary>
            /// Converts an UTF-16 encoded string to UTF-8.
            /// </summary>
            /// <param name="utf16String">The UTF-16 encoded string to be converted.</param>
            /// <returns>The converted UTF-8 encoded string.</returns>
            static std::string ConvertUTF16ToUTF8(const std::wstring_view& utf16String);
            /// <summary>
            /// Converts an UTF-8 encoded string to UTF-16.
            /// </summary>
            /// <param name="utf8String">The UTF-8 encoded string to be converted.</param>
            /// <returns>The converted UTF-16 encoded string.</returns>
            static std::wstring ConvertUTF8ToUTF16(const std::string_view& utf8String);

        private:
            Encoding() = delete;
        };
#endif

        /// <summary>
        /// Represents the terminal process.
        /// </summary>
        class Process
        {
        public:
            /// <summary>
            /// Gets command-line arguments of the terminal process.
            /// </summary>
            /// <param name="totalRawCMDArguments">The total of raw arguments given as the first parameter of the main function.</param>
            /// <param name="rawCMDArguments">The raw arguments given as the second parameter of the main function.</param>
            /// <returns>The arguments.</returns>
            static CMDArguments GetCMDArguments(int totalRawCMDArguments, const char** rawCMDArguments) noexcept;
            /// <summary>
            /// Exits the terminal process.
            /// </summary>
            /// <param name="code">The exit to be used.</param>
            static void Exit(unsigned char code) noexcept;
            /// <summary>
            /// Exits the terminal process.
            /// </summary>
            /// <param name="code">The exit to be used.</param>
            static void Exit(ExitCode code) noexcept;

        private:
            Process() = delete;
        };

        /// <summary>
        /// Represents the terminal input stream.
        /// </summary>
        class Input final : public Stream<0>
        {
        };

        /// <summary>
        /// Represents the terminal output stream.
        /// </summary>
        class Output final : public WritableStream<1>
        {
        public:
            /// <summary>
            /// Flushes the terminal output buffer.
            /// </summary>
            static void Flush() noexcept;
        };

        /// <summary>
        /// Represents the terminal error stream.
        /// </summary>
        class Error final : public WritableStream<2>
        {
        };

        /// <summary>
        /// Represents the terminal font.
        /// </summary>
        class Font final
        {
        public:
            /// <summary>
            /// Sets the terminal font weight.
            /// </summary>
            /// <param name="weight">The weight to be set.</param>
            static void SetWeight(FontWeight weight) noexcept;
            /// <summary>
            /// Sets a color from the XTerm 256 colors palette onto a terminal font layer.
            /// </summary>
            /// <param name="color">The code of the color.</param>
            /// <param name="layer">The layer where the should be applied on.</param>
            static void SetXTermColor(unsigned char color, FontLayer layer) noexcept;
            /// <summary>
            /// Sets a color from the XTerm 256 colors palette onto a terminal font layer.
            /// </summary>
            /// <param name="color">The code of the color.</param>
            /// <param name="layer">The layer where the should be applied on.</param>
            static void SetXTermColor(XTermColor color, FontLayer layer) noexcept;
            /// <summary>
            /// Resets the terminal font weight.
            /// </summary>
            static void ResetWeight() noexcept;
            /// <summary>
            /// Resets the terminal font colors.
            /// </summary>
            static void ResetColors() noexcept;

        private:
            Font() = delete;
        };
    };
#pragma endregion
}
