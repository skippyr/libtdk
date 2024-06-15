#include <exception>
#include <string>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

/// <summary>
/// Terminal Manipulation Kit (TMK) - An open-source C++ terminal manipulation library for Windows and Linux that includes features to handle terminal attributes, arguments and
/// events targetting UTF-8 encoding.
/// </summary>
namespace TMK
{
    /// <summary>
    /// Represents a byte.
    /// </summary>
    typedef char byte;

    /// <summary>
    /// Contains the POSIX exit codes.
    /// </summary>
    enum class ExitCode
    {
        /// <summary>
        /// Successful execution: thrown when the program execution succeeds.
        /// </summary>
        Success,
        /// <summary>
        /// Generic failed execution: thrown when the program execution had an error without further description.
        /// </summary>
        Failure,
        /// <summary>
        /// Operation not permitted (EPERM): thrown when the current user does not have enough privileges to complete an operation without further description.
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
        /// Input or output error (EIO): thrown when an input or output operation fails without further description.
        /// </summary>
        InputOutputErrorEIO,
        /// <summary>
        /// No such device or address (ENXIO): thrown when a non-existent device is referenced.
        /// </summary>
        NoSuchDeviceOrAddressENXIO,
        /// <summary>
        /// Argument list too long (E2BIG): thrown when a process is executed with more command line arguments than the system allows.
        /// </summary>
        ArgumentListTooLongE2BIG,
        /// <summary>
        /// Exec format error (ENOEXEC): thrown when a binary can not be executed without futher description.
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
        /// Resource temporarily unavailable (EWOULDBLOCK): thrown when a hardware or software resource is currently unavailable or can not be completed..
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
        /// Device or resource busy (EBUSY): thrown when a referenced resource is currently in use by a process.
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
        /// No message of desired type (ENOMSG): thrown when a message queue type does match a desired type.
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
        MemoryPageHasHardwareErrorEHWPOISON
    };

    /// <summary>
    /// Represents a generic exception.
    /// </summary>
    /// <typeparam name="T">The exit code related to the exception.</typeparam>
    template <ExitCode T>
    class Exception : public std::exception
    {
    public:
        /// <summary>
        /// Creates a new instance of the Exception class.
        /// </summary>
        /// <param name="description">The description of why the exception was thrown.</param>
        Exception(std::string description) noexcept;
        /// <summary>
        /// Gets the description of why the exception was thrown.
        /// </summary>
        /// <returns>The description of why the exception was thrown.</returns>
        std::string GetDescription() const noexcept;
        /// <summary>
        /// Gets the exit code related to the exception.
        /// </summary>
        /// <returns>The exit code related to the exception.</returns>
        ExitCode GetExitCode() const noexcept;
        /// <summary>
        /// Gets the description of why the exception was thrown.
        /// </summary>
        /// <returns>The description of why the exception was thrown.</returns>
        const char* what() const noexcept;

    private:
        /// <summary>
        /// The description of why the exception was thrown.
        /// </summary>
        std::string m_description;
    };

    /// <summary>
    /// Represents an exception thrown when a group of streams are redirected.
    /// </summary>
    class StreamRedirectionException final : public Exception<ExitCode::DeviceNotAStreamENOSTR>
    {
    public:
        /// <summary>
        /// Creates an instance of the StreamRedirectionException class.
        /// </summary>
        /// <param name="description">The description of why the exception was thrown.</param>
        StreamRedirectionException(std::string description) noexcept;
    };

    /// <summary>
    /// Represents an exception thrown when attributes of a terminal stream is invalid.
    /// </summary>
    class InvalidStreamAttributesException final : public Exception<ExitCode::InvalidArgumentEINVAL>
    {
    public:
        /// <summary>
        /// Creates an instance of the InvalidStreamAttributesException class.
        /// </summary>
        /// <param name="description">The description of why the exception was thrown.</param>
        InvalidStreamAttributesException(std::string description) noexcept;
    };

    class WideCharacterOrientationException final : public Exception<ExitCode::NoMessageOfDesiredTypeENOMSG>
    {
    };

    /// <summary>
    /// Represents the terminal.
    /// </summary>
    class Terminal final
    {
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

        private:
            /// <summary>
            /// Creates an instance of the Encoding class.
            /// </summary>
            Encoding() noexcept = delete;
        };
#endif

        /// <summary>
        /// Represents the terminal input stream.
        /// </summary>
        class Input final
        {
        public:
#ifdef _WIN32
            /// <summary>
            /// Gets the handle related to the terminal input stream.
            /// </summary>
            static HANDLE GetHandle() noexcept;
            /// <summary>
            /// Gets the mode of the terminal input stream.
            /// </summary>
            /// <exception cref="StreamRedirectionException">Thrown when the terminal input stream is redirected.</exception>
            static DWORD GetMode();
            /// <summary>
            /// Sets the mode of the terminal input stream.
            /// </summary>
            /// <param name="mode">The mode to be set.</param>
            /// <exception cref="StreamRedirectionException">Thrown when the terminal input stream is redirected.</exception>
            /// <exception cref="InvalidStreamAttributesException">Thrown when the mode is invalid.</exception>
            static void SetMode(DWORD mode);
#endif
            /// <summary>
            /// Gets the file ID related to the terminal input stream.
            /// </summary>
            static int GetFileID() noexcept;
            /// <summary>
            /// Checks if the terminal input stream is redirected.
            /// </summary>
            /// <returns>A boolean that states the terminal input stream is redirected.</returns>
            static bool IsRedirected() noexcept;
            /// <summary>
            /// Reads a byte from the terminal input buffer. If it is empty, it waits until the Enter key is pressed.
            /// </summary>
            /// <returns>The byte read.</returns>
            /// <exception cref="StreamRedirectionException">Thrown when the terminal input stream is redirected.</exception>
            /// <exception cref="WideCharacterOrientationException">Thrown whenever the terminal input stream is wide character oriented.</exception>
            static byte ReadByte();

        private:
            /// <summary>
            /// The name of the terminal input stream.
            /// </summary>
            static const std::string m_name;

            /// <summary>
            /// Creates an instance of the Input class.
            /// </summary>
            Input() noexcept = delete;
        };

        /// <summary>
        /// Represents the terminal output stream.
        /// </summary>
        class Output final
        {
        public:
#ifdef _WIN32
            /// <summary>
            /// Gets the handle related to the terminal output stream.
            /// </summary>
            static HANDLE GetHandle() noexcept;
            /// <summary>
            /// Gets the mode of the terminal output stream.
            /// </summary>
            /// <exception cref="StreamRedirectionException">Thrown when the terminal output stream is redirected.</exception>
            static DWORD GetMode();
            /// <summary>
            /// Sets the mode of the terminal output stream.
            /// </summary>
            /// <param name="mode">The mode to be set.</param>
            /// <exception cref="StreamRedirectionException">Thrown when the terminal output stream is redirected.</exception>
            /// <exception cref="InvalidStreamAttributesException">Thrown when the mode is invalid.</exception>
            static void SetMode(DWORD mode);
#endif
            /// <summary>
            /// Gets the file ID related to the terminal output stream.
            /// </summary>
            static int GetFileID() noexcept;
            /// <summary>
            /// Checks if the terminal output stream is redirected.
            /// </summary>
            /// <returns>A boolean that states the terminal output stream is redirected.</returns>
            static bool IsRedirected() noexcept;

        private:
            /// <summary>
            /// The name of the terminal output stream.
            /// </summary>
            static const std::string m_name;

            /// <summary>
            /// Creates an instance of the Output class.
            /// </summary>
            Output() noexcept = delete;
        };

        /// <summary>
        /// Represents the terminal error stream.
        /// </summary>
        class Error final
        {
        public:
#ifdef _WIN32
            /// <summary>
            /// Gets the handle related to the terminal error stream.
            /// </summary>
            static HANDLE GetHandle() noexcept;
            /// <summary>
            /// Gets the mode of the terminal error stream.
            /// </summary>
            /// <exception cref="StreamRedirectionException">Thrown when the terminal error stream is redirected.</exception>
            static DWORD GetMode();
            /// <summary>
            /// Sets the mode of the terminal error stream.
            /// </summary>
            /// <param name="mode">The mode to be set.</param>
            /// <exception cref="StreamRedirectionException">Thrown when the terminal error stream is redirected.</exception>
            /// <exception cref="InvalidStreamAttributesException">Thrown when the mode is invalid.</exception>
            static void SetMode(DWORD mode);
#endif
            /// <summary>
            /// Gets the file ID related to the terminal error stream.
            /// </summary>
            static int GetFileID() noexcept;
            /// <summary>
            /// Checks if the terminal error stream is redirected.
            /// </summary>
            /// <returns>A boolean that states the terminal error stream is redirected.</returns>
            static bool IsRedirected() noexcept;

        private:
            /// <summary>
            /// The name of the terminal error stream.
            /// </summary>
            static const std::string m_name;

            /// <summary>
            /// Creates an instance of the Error class.
            /// </summary>
            Error() noexcept = delete;
        };

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

        /// <summary>
        /// Creates an instance of the Terminal class.
        /// </summary>
        Terminal() noexcept = delete;

#ifdef _WIN32
        /// <summary>
        /// Initializes the virtual terminal processing: allowing it to parse ANSI escape sequences.
        /// </summary>
        static void InitializeVirtualTerminalProcessing() noexcept;
#endif

        /// <summary>
        /// Initializes the stream redirection cache.
        /// </summary>
        static void InitializeStreamRedirectionCache() noexcept;
        /// <summary>
        /// Gets the mode of a terminal stream.
        /// </summary>
        /// <param name="handle">The handle of the stream.</param>
        /// <param name="name">The name of the stream.</param>
        /// <exception cref="StreamRedirectionException">Thrown when the stream is redirected.</exception>
        static DWORD GetStreamMode(HANDLE handle, const std::string& name);
        /// <summary>
        /// Sets the mode of a terminal stream.
        /// </summary>
        /// <param name="handle">The handle of the stream.</param>
        /// <param name="isRedirected">A boolean that states the stream is redirected.</param>
        /// <param name="name">The name of the stream.</param>
        /// <param name="mode">The mode of the stream.</param>
        /// <exception cref="StreamRedirectionException">Thrown when the stream is redirected.</exception>
        /// <exception cref="InvalidStreamAttributesException">Thrown when the mode is invalid.</exception>
        static void SetStreamMode(HANDLE handle, bool isRedirected, const std::string& name, DWORD mode);
    };
}
