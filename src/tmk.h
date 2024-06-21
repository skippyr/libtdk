#ifndef _TMK_H
#define _TMK_H

#include <stdarg.h>
#include <stdbool.h>

#pragma region Enums
/**
 * @brief Contains the available exit codes.
 */
enum tmk_ExitCode
{
    /**
     * @brief Successful execution: thrown when the program execution succeeds.
     */
    tmk_ExitCode_Success,
    /**
     * @brief Generic failed execution: thrown when the program execution failed without further description.
     */
    tmk_ExitCode_Failure,
    /**
     * @brief Operation not permitted (EPERM): thrown when the current user does not have enough privileges to complete an operation.
     */
    tmk_ExitCode_OperationNotPermittedEPERM = 1,
    /**
     * @brief No such file or directory (ENOENT): thrown when a non-existent file system entry is referenced.
     */
    tmk_ExitCode_NoSuchFileOrDirectoryENOENT,
    /**
     * @brief No such process (ESRCH): thrown when a non-existent process is referenced.
     */
    tmk_ExitCode_NoSuchProcessESRCH,
    /**
     * @brief Interrupted system call (EINTR): thrown when a blocking system call is interrupted by a signal handler.
     */
    tmk_ExitCode_InterruptedSystemCallEINTR,
    /**
     * @brief Input or output error (EIO): thrown when an input or output operation fails.
     */
    tmk_ExitCode_InputOutputErrorEIO,
    /**
     * @brief No such device or address (ENXIO): thrown when an invalid device or device address is referenced.
     */
    tmk_ExitCode_NoSuchDeviceOrAddressENXIO,
    /**
     * @brief Argument list too long (E2BIG): thrown when the total arguments given to a process exceeds the system limit.
     */
    tmk_ExitCode_ArgumentListTooLongE2BIG,
    /**
     * @brief Exec format error (ENOEXEC): thrown when a binary can not be executed.
     */
    tmk_ExitCode_ExecFormatErrorENOEXEC,
    /**
     * @brief Bad file descriptor (EBADF): thrown when an invalid file descriptor is referenced.
     */
    tmk_ExitCode_BadFileDescriptorEBADF,
    /**
     * @brief No child processes (ECHILD): thrown when a group of processes referenced in a wait call does not exist or is not owned by the calling process.
     */
    tmk_ExitCode_NoChildProcessesECHILD,
    /**
     * @brief Resource temporarily unavailable (EAGAIN): thrown when a hardware or software resource is currently unavailable or can not be completed.
     */
    tmk_ExitCode_ResourceTemporarilyUnavailableEAGAIN,
    /**
     * @brief Resource temporarily unavailable (EWOULDBLOCK): thrown when a hardware or software resource is currently unavailable or can not be completed.
     */
    tmk_ExitCode_ResourceTemporarilyUnavailableEWOULDBLOCK = 11,
    /**
     * @brief Can not allocate memory (ENOMEM): thrown when the system does not have more memory available.
     */
    tmk_ExitCode_CanNotAllocateMemoryENOMEM,
    /**
     * @brief Permission denied (EACCES): thrown when the current user does not have enough privileges to complete a file system operation.
     */
    tmk_ExitCode_PermissionDeniedEACCES,
    /**
     * @brief Bad address (EFAULT): thrown when a process attemps to access a not owned memory address.
     */
    tmk_ExitCode_BadAddressEFAULT,
    /**
     * @brief Block device required (ENOTBLK): thrown when an invalid resource is reference in place of a block device.
     */
    tmk_ExitCode_BlockDeviceRequiredENOTBLK,
    /**
     * @brief Device or resource busy (EBUSY): thrown when a resource is currently in use by a process.
     */
    tmk_ExitCode_DeviceOrResourceBusyEBUSY,
    /**
     * @brief File exists (EEXIST): thrown when a file system entry already exists.
     */
    tmk_ExitCode_FileExistsEEXIST,
    /**
     * @brief Invalid cross-device link (EXDEV): thrown when a file transfer operating between two file systems fails.
     */
    tmk_ExitCode_InvalidCrossDeviceLinkEXDEV,
    /**
     * @brief No such device (ENODEV): thrown when a non-existent device is referenced.
     */
    tmk_ExitCode_NoSuchDeviceENODEV,
    /**
     * @brief Not a directory (ENOTDIR): thrown when an invalid file system entry is referenced in place of a directory.
     */
    tmk_ExitCode_NotADirectoryENOTDIR,
    /**
     * @brief Is a directory (EISDIR): thrown when an invalid operation is attemped on a directory.
     */
    tmk_ExitCode_IsADirectoryEISDIR,
    /**
     * @brief Invalid argument (EINVAL): thrown when an operation is attemped with invalid arguments.
     */
    tmk_ExitCode_InvalidArgumentEINVAL,
    /**
     * @brief Too many open files in system (ENFILE): thrown when the total of open file descriptors has exceeded the system-wide limit.
     */
    tmk_ExitCode_TooManyOpenFilesInSystemENFILE,
    /**
     * @brief Too many open files (EMFILE): thrown when the total of open file descriptors has exceeded the process limit.
     */
    tmk_ExitCode_TooManyOpenFilesEMFILE,
    /**
     * @brief Inappropriate ioctl for device (ENOTTY): thrown when an input or output operation is attemped on a device that does not support it.
     */
    tmk_ExitCode_InappropriateIoctlForDeviceENOTTY,
    /**
     * @brief Text file busy (ETXTBSY): thrown when a file descriptor is in use by a process.
     */
    tmk_ExitCode_TextFileBusyETXTBSY,
    /**
     * @brief File too large (EFBIG): thrown when a write operation fails due to the system file size limit has been reached.
     */
    tmk_ExitCode_FileTooLargeEFBIG,
    /**
     * @brief No space left on device (ENOSPC): thrown when a device does not have space available.
     */
    tmk_ExitCode_NoSpaceLeftOnDeviceENOSPC,
    /**
     * @brief Illegal seek (ESPIPE): thrown when a seek operation is attemped on a file descriptor that does not supports it.
     */
    tmk_ExitCode_IllegalSeekESPIPE,
    /**
     * @brief Read-only file system (EROFS): thrown when a write operation is attemped on a read-only file system.
     */
    tmk_ExitCode_ReadOnlyFileSystemEROFS,
    /**
     * @brief Too many links (EMLINK): thrown when a file reaches the system limit of hard links.
     */
    tmk_ExitCode_TooManyLinksEMLINK,
    /**
     * @brief Broken pipe (EPIPE): thrown when a write operation is attemped on a closed pipe.
     */
    tmk_ExitCode_BrokenPipeEPIPE,
    /**
     * @brief Numerical argument out of domain (EDOM): thrown when an argument of invalid domain is used in a math operation.
     */
    tmk_ExitCode_NumericalArgumentOutOfDomainEDOM,
    /**
     * @brief Numerical result out of range (ERANGE): thrown when a value can not be represented within a given range.
     */
    tmk_ExitCode_NumericalResultOutOfRangeERANGE,
    /**
     * @brief Resource deadlock avoided (EDEADLK): thrown when two processes depend on resources of each other to complete an operation.
     */
    tmk_ExitCode_ResourceDeadlockAvoidedEDEADLK,
    /**
     * @brief Resource deadlock avoided (EDEADLOCK): thrown when two processes depend on resources of each other to complete an operation.
     */
    tmk_ExitCode_ResourceDeadlockAvoidedEDEADLOCK = 35,
    /**
     * @brief File name too long (ENAMETOOLONG): thrown when a file system entry has a name that exceeds the system limit.
     */
    tmk_ExitCode_FileNameTooLongENAMETOOLONG,
    /**
     * @brief No locks available (ENOLCK): thrown when there are no NFS locks available.
     */
    tmk_ExitCode_NoLocksAvailableENOLCK,
    /**
     * @brief Function not implemented (ENOSYS): thrown when a non-existent system call is referenced.
     */
    tmk_ExitCode_FunctionNotImplementedENOSYS,
    /**
     * @brief Directory not empty (ENOTEMPTY): thrown an operation can not be performed on a non-empty directory.
     */
    tmk_ExitCode_DirectoryNotEmptyENOTEMPTY,
    /**
     * @brief Too many levels of symbolic links (ELOOP): thrown when an operation can not be performed due to recursive calls on the same symbolic link.
     */
    tmk_ExitCode_TooManyLevelsOfSymbolicLinksELOOP,
    /**
     * @brief No message of desired type (ENOMSG): thrown when a message queue does match the desired type.
     */
    tmk_ExitCode_NoMessageOfDesiredTypeENOMSG = 42,
    /**
     * @brief Identifier removed (EIDRM): thrown when an inter-process communication resource is removed during an operation.
     */
    tmk_ExitCode_IdentifierRemovedEIDRM,
    /**
     * @brief Channel number out of range (ECHRNG): thrown when an invalid device channel number is referenced.
     */
    tmk_ExitCode_ChannelNumberOutOfRangeECHRNG,
    /**
     * @brief Level 2 not synchronized (EL2NSYNC): thrown when a netlink operation fails due to a synchronization issue.
     */
    tmk_ExitCode_Level2NotSynchronizedEL2NSYNC,
    /**
     * @brief Level 3 halted (EL3HLT): thrown when a required hardware state is not available.
     */
    tmk_ExitCode_Level3HaltedEL3HLT,
    /**
     * @brief Level 3 reset (EL3RST): thrown when a device or subsystem reset fails.
     */
    tmk_ExitCode_Level3ResetEL3RST,
    /**
     * @brief Link number out of range (ELNRNG): thrown when an invalid hard or symbolic link is referenced.
     */
    tmk_ExitCode_LinkNumberOutOfRangeELNRNG,
    /**
     * @brief Protocol driver not attached (EUNATCH): thrown when a terminal device is not attached to a protocol driver.
     */
    tmk_ExitCode_ProtocolDriverNotAttachedEUNATCH,
    /**
     * @brief No CSI structure available (ENOCSI): thrown when a terminal CSI structure is not available or valid.
     */
    tmk_ExitCode_NoCSIStructureAvailableENOCSI,
    /**
     * @brief Level 2 halted (EL2HLT): thrown when the requested network protocol is unavailable in the system.
     */
    tmk_ExitCode_Level2HaltedEL2HLT,
    /**
     * @brief Invalid exchange (EBADE): thrown when an invalid exchange is specified to a system call.
     */
    tmk_ExitCode_InvalidExchangeEBADE,
    /**
     * @brief Invalid request descriptor (EBADR): thrown when an invalid request descriptor is specified to a system call.
     */
    tmk_ExitCode_InvalidRequestDescriptorEBADR,
    /**
     * @brief Exchange full (EXFULL): thrown when a communication exchange queue is full.
     */
    tmk_ExitCode_ExchangeFullEXFULL,
    /**
     * @brief No anode (ENOANO): thrown when a non-existent anode is referenced.
     */
    tmk_ExitCode_NoAnodeENOANO,
    /**
     * @brief Invalid request code (EBADRQC): thrown when an invalid system call request code is used.
     */
    tmk_ExitCode_InvalidRequestCodeEBADRQC,
    /**
     * @brief Invalid slot (EBADSLT): thrown when an invalid slot number is specified in a system call.
     */
    tmk_ExitCode_InvalidSlotEBADSLT,
    /**
     * @brief Bad font file format (EBFONT): thrown when an font file is invalid or corrupted.
     */
    tmk_ExitCode_BadFontFileFormatEBFONT = 59,
    /**
     * @brief Device not a stream (ENOSTR): thrown when a file descriptor referenced does not support streaming operations.
     */
    tmk_ExitCode_DeviceNotAStreamENOSTR,
    /**
     * @brief No data available (ENODATA): thrown when no data is available during a non-blocking read operation.
     */
    tmk_ExitCode_NoDataAvailableENODATA,
    /**
     * @brief Timer expired (ETIME): thrown when a timer associated to an operation expires before it has been completed.
     */
    tmk_ExitCode_TimerExpiredETIME,
    /**
     * @brief Out of streams resources (ENOSR): thrown when the system can not allocate resources for streaming operations.
     */
    tmk_ExitCode_OutOfStreamsResourcesENOSR,
    /**
     * @brief Machine is not on the network (ENONET): thrown when an operation fails due to the machine not being connected to the required network.
     */
    tmk_ExitCode_MachineIsNotOnTheNetworkENONET,
    /**
     * @brief Package not installed (ENOPKG): thrown when a package or component is not installed in the system.
     */
    tmk_ExitCode_PackageNotInstalledENOPKG,
    /**
     * @brief Object is remote (EREMOTE): thrown when an object referred is on a remote system.
     */
    tmk_ExitCode_ObjectIsRemoteEREMOTE,
    /**
     * @brief Link has been severed (ENOLINK): thrown when the connection to a resource has been closed.
     */
    tmk_ExitCode_LinkHasBeenSeveredENOLINK,
    /**
     * @brief Advertise error (EADV): thrown when a connection advertisement fails.
     */
    tmk_ExitCode_AdvertiseErrorEADV,
    /**
     * @brief Srmount error (ESRMNT): thrown when mouting a device using the srmount feature fails.
     */
    tmk_ExitCode_SrmountErrorESRMNT,
    /**
     * @brief Communication error on send (ECOMM): thrown when communication fails during a send operation.
     */
    tmk_ExitCode_CommunicationErrorOnSendECOMM,
    /**
     * @brief Protocol error (EPROTO): thrown when there is an error at the network protocol.
     */
    tmk_ExitCode_ProtocolErrorEPROTO,
    /**
     * @brief Multihop attempted (EMULTIHOP): thrown when an operation attemps routing hopping without permissions or support.
     */
    tmk_ExitCode_MultihopAttemptedEMULTIHOP,
    /**
     * @brief RFS specific error (EDOTDOT): thrown when there is an error in the RFS system.
     */
    tmk_ExitCode_RFSSpecificErrorEDOTDOT,
    /**
     * @brief Bad message (EBADMSG): thrown when a message received through a connection does not match its protocol.
     */
    tmk_ExitCode_BadMessageEBADMSG,
    /**
     * @brief Value too large for defined data type (EOVERFLOW): thrown when a value can not be hold by its data type.
     */
    tmk_ExitCode_ValueTooLargeForDefinedDataTypeEOVERFLOW,
    /**
     * @brief Name not unique on network (ENOTUNIQ): thrown when the name provided to perform a network operation is not unique.
     */
    tmk_ExitCode_NameNotUniqueOnNetworkENOTUNIQ,
    /**
     * @brief File descriptor in bad state (EBADFD): thrown when an operation is attemped on an invalid file descriptor.
     */
    tmk_ExitCode_FileDescriptorInBadStateEBADFD,
    /**
     * @brief Remote address changed (EREMCHG): thrown when a remote address is changed during connection.
     */
    tmk_ExitCode_RemoteAddressChangedEREMCHG,
    /**
     * @brief Can not access a needed shared library (ELIBACC): thrown when a shared library can not be found or accessed.
     */
    tmk_ExitCode_CanNotAccessANeededSharedLibraryELIBACC,
    /**
     * @brief Accessing a corrupted shared library (ELIBBAD): thrown when a shared library is invalid or corrupted.
     */
    tmk_ExitCode_AccessingACorruptedSharedLibraryELIBBAD,
    /**
     * @brief .lib section in a.out corrupted (ELIBSCN): thrown when the .lib assembly section of an executable is invalid or corrupted.
     */
    tmk_ExitCode_LibSectionInAOutCorruptedELIBSCN,
    /**
     * @brief Attempting to link in too many shared libraries (ELIBMAX): thrown when the total of shared libraries being linked into an executable exceeds the system limit.
     */
    tmk_ExitCode_AttemptingToLinkInTooManySharedLibrariesELIBMAX,
    /**
     * @brief Cannot exec a shared library directly (ELIBEXEC): thrown when a shared library is attemped to be executed directly.
     */
    tmk_ExitCode_CannotExecASharedLibraryDirectlyELIBEXEC,
    /**
     * @brief Invalid or incomplete multibyte or wide character (EILSEQ): thrown when a byte sequence is invalid given the expected encoding.
     */
    tmk_ExitCode_InvalidOrIncompleteMultibyteOrWideCharacterEILSEQ,
    /**
     * @brief Interrupted system call should be restarted (ERESTART): thrown when a system call is interrupted by a signal handler and must be attemped again.
     */
    tmk_ExitCode_InterruptedSystemCallShouldBeRestartedERESTART,
    /**
     * @brief Streams pipe error (ESTRPIPE): thrown when a write operation is perfomed on a closed stream.
     */
    tmk_ExitCode_StreamsPipeErrorESTRPIPE,
    /**
     * @brief Too many users (EUSERS): thrown when the total of network connections or users exceeds the system limit.
     */
    tmk_ExitCode_TooManyUsersEUSERS,
    /**
     * @brief Socket operation on non-socket (ENOTSOCK): thrown when an invalid file system entry is referenced in place of a socket.
     */
    tmk_ExitCode_SocketOperationOnNonSocketENOTSOCK,
    /**
     * @brief Destination address required (EDESTADDRREQ): thrown when no destination address is provided for a network operation.
     */
    tmk_ExitCode_DestinationAddressRequiredEDESTADDRREQ,
    /**
     * @brief Message too long (EMSGSIZE): thrown when the message sent through a socket exceeds the limit predefined by its protocol.
     */
    tmk_ExitCode_MessageTooLongEMSGSIZE,
    /**
     * @brief Protocol wrong type for socket (EPROTOTYPE): thrown when a protocol does match the socket it is attemped on.
     */
    tmk_ExitCode_ProtocolWrongTypeForSocketEPROTOTYPE,
    /**
     * @brief Protocol not available (ENOPROTOOPT): thrown when a protocol requested is not available in the system.
     */
    tmk_ExitCode_ProtocolNotAvailableENOPROTOOPT,
    /**
     * @brief Protocol not supported (EPROTONOSUPPORT): thrown when a protocol is not available in the system.
     */
    tmk_ExitCode_ProtocolNotSupportedEPROTONOSUPPORT,
    /**
     * @brief Socket type not supported (ESOCKTNOSUPPORT): thrown a the socket type is not available in the system.
     */
    tmk_ExitCode_SocketTypeNotSupportedESOCKTNOSUPPORT,
    /**
     * @brief Operation not supported (EOPNOTSUPP): thrown when a socket or its protocol do not support an operation.
     */
    tmk_ExitCode_OperationNotSupportedEOPNOTSUPP,
    /**
     * @brief Operation not supported (ENOTSUP): thrown when a socket or its protocol do not support an operation.
     */
    tmk_ExitCode_OperationNotSupportedENOTSUP = 95,
    /**
     * @brief Protocol family not supported (EPFNOSUPPORT): thrown when a protocol family is not available in the system.
     */
    tmk_ExitCode_ProtocolFamilyNotSupportedEPFNOSUPPORT,
    /**
     * @brief Address family not supported by protocol (EAFNOSUPPORT): thrown when an invalid address family is used in a protocol.
     */
    tmk_ExitCode_AddressFamilyNotSupportedByProtocolEAFNOSUPPORT,
    /**
     * @brief Address already in use (EADDRINUSE): thrown when a socket is attemped to be bind to an address that is already in use.
     */
    tmk_ExitCode_AddressAlreadyInUseEADDRINUSE,
    /**
     * @brief Cannot assign requested address (EADDRNOTAVAIL): thrown when an address can not be bind to a socket due to it is being unavailable.
     */
    tmk_ExitCode_CannotAssignRequestedAddressEADDRNOTAVAIL,
    /**
     * @brief Network is down (ENETDOWN): thrown when the network subsystem is unavailable.
     */
    tmk_ExitCode_NetworkIsDownENETDOWN,
    /**
     * @brief Network is unreachable (ENETUNREACH): thrown when no routing could be found to reach a network.
     */
    tmk_ExitCode_NetworkIsUnreachableENETUNREACH,
    /**
     * @brief Network dropped connection on reset (ENETRESET): thrown when a connection is closed due to a reset.
     */
    tmk_ExitCode_NetworkDroppedConnectionOnResetENETRESET,
    /**
     * @brief Software caused connection abort (ECONNABORTED): thrown when a connection is closed due to a software crash.
     */
    tmk_ExitCode_SoftwareCausedConnectionAbortECONNABORTED,
    /**
     * @brief Connection reset by peer (ECONNRESET): thrown when the connection is forcibly closed by one of its peer.
     */
    tmk_ExitCode_ConnectionResetByPeerECONNRESET,
    /**
     * @brief No buffer space available (ENOBUFS): thrown when the total of network buffers exceeds the system limit.
     */
    tmk_ExitCode_NoBufferSpaceAvailableENOBUFS,
    /**
     * @brief Transport endpoint is already connected (EISCONN): thrown when a connection is attemped on a socket that is already connected.
     */
    tmk_ExitCode_TransportEndpointIsAlreadyConnectedEISCONN,
    /**
     * @brief Transport endpoint is not connected (ENOTCONN): thrown when an operation is attemped on a non-connected socket.
     */
    tmk_ExitCode_TransportEndpointIsNotConnectedENOTCONN,
    /**
     * @brief Can not send after transport endpoint shutdown (ESHUTDOWN): thrown when an operation is attemped on a socket that has been shutdown.
     */
    tmk_ExitCode_CanNotSendAfterTransportEndpointShutdownESHUTDOWN,
    /**
     * @brief Too many references: cannot splice (ETOOMANYREFS): thrown when there are too many references to a kernel resource.
     */
    tmk_ExitCode_TooManyReferencesCannotSpliceETOOMANYREFSs,
    /**
     * @brief Connection timed out (ETIMEDOUT): thrown when a connection can not be stablished in its protocol predefined time.
     */
    tmk_ExitCode_ConnectionTimedOutETIMEDOUT,
    /**
     * @brief Connection refused (ECONNREFUSED): thrown when a connection can not be stablished due to no host been listening to it.
     */
    tmk_ExitCode_ConnectionRefusedECONNREFUSED,
    /**
     * @brief Host is down (EHOSTDOWN): thrown when a connection can not be stablished because the host being reached is down.
     */
    tmk_ExitCode_HostIsDownEHOSTDOWN,
    /**
     * @brief No route to host (EHOSTUNREACH): thrown when no routing could be found to reach a host inside of a network.
     */
    tmk_ExitCode_NoRouteToHostEHOSTUNREACH,
    /**
     * @brief Operation already in progress (EALREADY): thrown when a non-blocking device has another operation currently queue.
     */
    tmk_ExitCode_OperationAlreadyInProgressEALREADY,
    /**
     * @brief Operation now in progress (EINPROGRESS): thrown when a non-blocking device is already running a same operation.
     */
    tmk_ExitCode_OperationNowInProgressEINPROGRESS,
    /**
     * @brief Stale file handle (ESTALE): thrown when a NFS file handle stales and becomes invalid.
     */
    tmk_ExitCode_StaleFileHandleESTALE,
    /**
     * @brief Structure needs cleaning (EUCLEAN): thrown when the file system structure needs cleaning.
     */
    tmk_ExitCode_StructureNeedsCleaningEUCLEAN,
    /**
     * @brief Not a XENIX named type file (ENOTNAM): thrown when an invalid file system entry is referenced in place of a named type file.
     */
    tmk_ExitCode_NotAXENIXNamedTypeFileENOTNAM,
    /**
     * @brief No XENIX semaphores available (ENAVAIL): thrown when the total of semaphores exceeds the system limit.
     */
    tmk_ExitCode_NoXENIXSemaphoresAvailableENAVAIL,
    /**
     * @brief Is a named type file (EISNAM): thrown when a named type file is referenced in place of a non-named type file.
     */
    tmk_ExitCode_IsANamedTypeFileEISNAM,
    /**
     * @brief Remote I/O error (EREMOTEIO): thrown when an input or output error has happened when accessing a remote file system.
     */
    tmk_ExitCode_RemoteIOErrorEREMOTEIO,
    /**
     * @brief Disk quota exceeded (EDQUOT): thrown when a write operation is attemped while the disk quota exceeds its limit.
     */
    tmk_ExitCode_DiskQuotaExceededEDQUOT,
    /**
     * @brief No medium found (ENOMEDIUM): thrown when no medium is found in the drive.
     */
    tmk_ExitCode_NoMediumFoundENOMEDIUM,
    /**
     * @brief Wrong medium type (EMEDIUMTYPE): thrown when the inserted medium is not supported by the drive or it is not formatted.
     */
    tmk_ExitCode_WrongMediumTypeEMEDIUMTYPE,
    /**
     * @brief Operation canceled (ECANCELED): thrown when an asynchronous operation is canceled before it has been completed.
     */
    tmk_ExitCode_OperationCanceledECANCELED,
    /**
     * @brief Required key not available (ENOKEY): thrown when a cryptographic key is not available.
     */
    tmk_ExitCode_RequiredKeyNotAvailableENOKEY,
    /**
     * @brief Key has expired (EKEYEXPIRED): thrown when a cryptographic key has expired.
     */
    tmk_ExitCode_KeyHasExpiredEKEYEXPIRED,
    /**
     * @brief Key has been revoked (EKEYREVOKED): thrown when a cryptographic key has been revoked.
     */
    tmk_ExitCode_KeyHasBeenRevokedEKEYREVOKED,
    /**
     * @brief Key was rejected by service (EKEYREJECTED): thrown when a cryptographic key is rejected by a service due to not match its criteria.
     */
    tmk_ExitCode_KeyWasRejectedByServiceEKEYREJECTED,
    /**
     * @brief Owner died (EOWNERDEAD): thrown when the process owner of a mutex dies without releasing it.
     */
    tmk_ExitCode_OwnerDiedEOWNERDEAD,
    /**
     * @brief State not recoverable (ENOTRECOVERABLE): thrown when the previous state of a mutex can not be restored after it has been unlocked.
     */
    tmk_ExitCode_StateNotRecoverableENOTRECOVERABLE,
    /**
     * @brief Operation not possible due to RF-kill (ERFKILL): thrown when an operation is canceled by the RF-kill feature.
     */
    tmk_ExitCode_OperationNotPossibleDueToRFKillERFKILL,
    /**
     * @brief Memory page has hardware error (EHWPOISON): thrown when a memory page gets corrupted.
     */
    tmk_ExitCode_MemoryPageHasHardwareErrorEHWPOISON
};

/**
 * @brief Contains the terminal streams.
 */
enum tmk_Stream
{
    /**
     * @brief Where events and data is input.
     */
    tmk_Stream_Input,
    /**
     * @brief Where regular tense messages are output.
     */
    tmk_Stream_Output,
    /**
     * @brief Where error tense messages are output.
     */
    tmk_Stream_Error
};

/**
 * @brief Contains the ANSI code of the first 16 colors of the XTerm palette.
 */
enum tmk_XColor
{
    /**
     * @brief The dark variant of the black color.
     */
    tmk_XColor_DarkBlack,
    /**
     * @brief The dark variant of the red color.
     */
    tmk_XColor_DarkRed,
    /**
     * @brief The dark variant of the green color.
     */
    tmk_XColor_DarkGreen,
    /**
     * @brief The dark variant of the yellow color.
     */
    tmk_XColor_DarkYellow,
    /**
     * @brief The dark variant of the blue color.
     */
    tmk_XColor_DarkBlue,
    /**
     * @brief The dark variant of the magenta color.
     */
    tmk_XColor_DarkMagenta,
    /**
     * @brief The dark variant of the cyan color.
     */
    tmk_XColor_DarkCyan,
    /**
     * @brief The dark variant of the white color.
     */
    tmk_XColor_DarkWhite,
    /**
     * @brief The light variant of the black color.
     */
    tmk_XColor_LightBlack,
    /**
     * @brief The light variant of the red color.
     */
    tmk_XColor_LightRed,
    /**
     * @brief The light variant of the green color.
     */
    tmk_XColor_LightGreen,
    /**
     * @brief The light variant of the yellow color.
     */
    tmk_XColor_LightYellow,
    /**
     * @brief The light variant of the blue color.
     */
    tmk_XColor_LightBlue,
    /**
     * @brief The light variant of the magenta color.
     */
    tmk_XColor_LightMagenta,
    /**
     * @brief The light variant of the cyan color.
     */
    tmk_XColor_LightCyan,
    /**
     * @brief The light variant of the white color.
     */
    tmk_XColor_LightWhite,
};

/**
 * @brief Contains the terminal font layers.
 */
enum tmk_FontLayer
{
    /**
     * @brief Refers to the graphemes.
     */
    tmk_FontLayer_Foreground = 3,
    /**
     * @brief Refers to the background of the graphemes.
     */
    tmk_FontLayer_Background
};

/**
 * @brief Contains the terminal font weights.
 */
enum tmk_FontWeight
{
    /**
     * @brief Usually rendered as bold weight and/or with bright colors.
     */
    tmk_FontWeight_Bold = 1,
    /**
     * @brief Usually rendered with faint colors.
     */
    tmk_FontWeight_Light
};

/**
 * @brief Contains the terminal font effects.
 */
enum tmk_FontEffect
{
    /**
     * @brief Makes the text curly.
     */
    tmk_FontEffect_Italic = 1 << 3,
    /**
     * @brief Draws a horizontal line crossing below the text.
     */
    tmk_FontEffect_Underline = 1 << 4,
    /**
     * @brief Makes the text blink indefinitely.
     */
    tmk_FontEffect_Blink = 1 << 5,
    /**
     * @brief Swaps the background and foreground colors.
     */
    tmk_FontEffect_Negative = 1 << 7,
    /**
     * @brief Makes the text hard to see or invisible.
     */
    tmk_FontEffect_Hidden = 1 << 8,
    /**
     * @brief Draws a horizontal line crossing through the middle of the text.
     */
    tmk_FontEffect_Strike = 1 << 9
};

/**
 * @brief Contains the terminal cursor shapes.
 */
enum tmk_CursorShape
{
    /**
     * @brief Fills the whole character cell.
     **/
    tmk_CursorShape_Block = 2,
    /**
     * @brief Fills a region at the bottom of the character cell.
     **/
    tmk_CursorShape_Underline = 4,
    /**
     * @brief Fills a region at the left of the character cell.
     */
    tmk_CursorShape_Bar = 6
};
#pragma endregion

#pragma region Structs
/**
 * @brief Represents an RGB color.
 */
struct tmk_RGBColor
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

/**
 * @brief Represents terminal dimensions.
 */
struct tmk_Dimensions
{
    /**
     * @brief The total columns of the dimensions.
     */
    unsigned short totalColumns;
    /**
     * @brief The total rows of the dimensions.
     */
    unsigned short totalRows;
};
#pragma endregion

#ifdef __cplusplus
extern "C"
{
#endif
#pragma region Functions
    /**
     * @brief Checks if a terminal stream is redirected.
     * @param stream The stream being referenced.
     * @returns A boolean that states the terminal stream is redirected.
     */
    bool tmk_isStreamRedirected(enum tmk_Stream stream);
    /**
     * @brief Gets the terminal window dimensions.
     * @param dimensions The address where the information about the dimensions should be put into.
     * @returns 0 if successful or -1 otherwise.
     */
    int tmk_getWindowDimensions(struct tmk_Dimensions* dimensions);
    /**
     * @brief Sets a XTerm color into a terminal font layer.
     * @param color The color to be set. It must be a value in range from 0 to 255 or an enumerator from the tmk_XColor enum.
     * @param layer The layer to be affected.
     */
    void tmk_setFontXColor(unsigned char color, enum tmk_FontLayer layer);
    /**
     * @brief Sets an RGB color into a terminal font layer.
     * @param color The color to be set.
     * @param layer The layer to be affected.
     */
    void tmk_setFontRGBColor(struct tmk_RGBColor color, enum tmk_FontLayer layer);
    /**
     * @brief Sets a hex color into a terminal font layer.
     * @param color The color to be set. It must be a value in range from 0x0 to 0xffffff.
     * @param layer The layer to be affected.
     */
    void tmk_setFontHexColor(unsigned int color, enum tmk_FontLayer layer);
    /**
     * @brief Sets the terminal font weight.
     * @param weight The weight to be set.
     */
    void tmk_setFontWeight(enum tmk_FontWeight weight);
    /**
     * @brief Sets terminal font effects.
     * @param effectsMask A bitmask containing the effects to be set.
     */
    void tmk_setFontEffects(int effectsMask);
    /**
     * @brief Sets the terminal cursor shape.
     * @param shape The shape to be set.
     * @param shouldBlink A boolean that states the cursor should blink.
     */
    void tmk_setCursorShape(enum tmk_CursorShape shape, bool shouldBlink);
    /**
     * @brief Sets the terminal cursor visibility.
     * @param isVisible A boolean that states the terminal cursor should be visible.
     */
    void tmk_setCursorVisibility(bool isVisible);
    /**
     * @brief Resets the terminal font colors.
     */
    void tmk_resetFontColors(void);
    /**
     * @brief Resets the terminal font weight.
     */
    void tmk_resetFontWeight(void);
    /**
     * @brief Resets the terminal font effects.
     */
    void tmk_resetFontEffects(void);
    /**
     * @brief Resets the terminal cursor shape.
     */
    void tmk_resetCursorShape(void);
    /**
     * @brief Opens the alternate window buffer.
     */
    void tmk_openAlternateWindow(void);
    /**
     * @brief Closes the alternate window buffer.
     */
    void tmk_closeAlternateWindow(void);
    /**
     * @brief Formats and writes a string to the terminal error stream.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param arguments The arguments to be formatted.
     */
    void tmk_writeErrorArguments(const char* format, va_list arguments);
    /**
     * @brief Formats and writes a string to the terminal error stream.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param ... The arguments to be formatted.
     */
    void tmk_writeError(const char* format, ...);
    /**
     * @brief Formats and writes a string to the terminal error stream with a newline grapheme appended to its end.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param arguments The arguments to be formatted.
     */
    void tmk_writeErrorLineArguments(const char* format, va_list arguments);
    /**
     * @brief Formats and writes a string to the terminal error stream with a newline grapheme appended to its end.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param ... The arguments to be formatted.
     */
    void tmk_writeErrorLine(const char* format, ...);
    /**
     * @brief Formats and writes a string to the terminal output stream.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param arguments The arguments to be formatted.
     */
    void tmk_writeArguments(const char* format, va_list arguments);
    /**
     * @brief Formats and writes a string to the terminal output stream.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param ... The arguments to be formatted.
     */
    void tmk_write(const char* format, ...);
    /**
     * @brief Formats and writes a string to the terminal output stream with a newline grapheme appended to its end.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param arguments The arguments to be formatted.
     */
    void tmk_writeLineArguments(const char* format, va_list arguments);
    /**
     * @brief Formats and writes a string to the terminal output stream with a newline grapheme appended to its end.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param ... The arguments to be formatted.
     */
    void tmk_writeLine(const char* format, ...);
#pragma endregion
#ifdef __cplusplus
}
#endif
#endif
