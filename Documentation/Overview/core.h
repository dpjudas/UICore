
/*! \page Core Core Module Reference

	<h2>Platform abstraction</h2>

	<ul>
		<li>uicore::System - General system helper functions</li>
		<li>uicore::RegistryKey - C++ class for accessing the Windows registry</li>
		<li>uicore::ComPtr - Smart pointer template for Component Object Model (COM) objects</li>
	</ul>

	<h2>General utility classes</h2>
		
	<ul>
		<li>uicore::Exception - Base class for all exceptions thrown by ClanLib</li>
		<li>uicore::DataBuffer - General purpose memory buffer class</li>
		<li>uicore::DisposableObject - Helper for objects that may outlive their disposable resources</li>
		<li>uicore::DateTime - Date/time handling class</li>
	</ul>
		
	<h2>Signals</h2>

	<ul>
		<li>uicore::Signal</li>
		<li>uicore::SlotContainer</li>
	</ul>

	<h2>Math</h2>
		
	<ul>
		<li>uicore::Vec4, uicore::Vec4f, uicore::Vec4d, uicore::Vec4i, uicore::Vec4ui, uicore::Vec4s, uicore::Vec4us, uicore::Vec4ub, uicore::Vec4b - Vector templates for 4D</li>
		<li>uicore::Vec3, uicore::Vec3f, uicore::Vec3d, uicore::Vec3i, uicore::Vec3ui, uicore::Vec3s, uicore::Vec3us, uicore::Vec3ub, uicore::Vec3b - Vector templates for 3D</li>
		<li>uicore::Vec2, uicore::Vec2f, uicore::Vec2d, uicore::Vec2i, uicore::Vec2ui, uicore::Vec2s, uicore::Vec2us, uicore::Vec2ub, uicore::Vec2b - Vector templates for 2D</li>
		<li>uicore::HalfFloat, uicore::Vec4hf, uicore::Vec3hf, uicore::Vec2hf - Half-float converters</li>
		<li>uicore::Mat4, uicore::Mat4f, uicore::Mat4d, uicore::Mat4i - 4x4 Matrix templates</li>
		<li>uicore::Mat3, uicore::Mat3f, uicore::Mat3d, uicore::Mat3i - 3x3 Matrix templates</li>
		<li>uicore::Mat2, uicore::Mat2f, uicore::Mat2d, uicore::Mat2i - 2x2 Matrix templates</li>
		<li>uicore::Quaternionf, uicore::Quaterniond - Quaternion rotation/orientation classes</li>
		<li>uicore::Rect, uicore::Rectf, uicore::Rectd - 2D (left,top,right,bottom) rectangle structure</li>
		<li>uicore::Point, uicore::Pointf, uicore::Pointd - 2D (x,y) point structure</li>
		<li>uicore::Size, uicore::Sizef, uicore::Sized - 2D (width,height) size structure</li>
	</ul>
		
	<h2>Text</h2>
		
	<p>All strings are assumed to be UTF-8, unless otherwise specified.  std::string is used as the main string class.</p>
		
	<ul>
		<li>uicore::Text - Misc string helper functions</li>
		<li>uicore::UTF8_Reader - Converts UTF-8 to 32-bit unicode characters</li>
		<li>uicore::StringFormat, uicore::string_format - String formatting class and functions</li>
	</ul>
		
	<h2>XML</h2>

	<ul>
		<li>uicore::XmlTokenizer - Breaks a XML file into tokens</li>
		<li>uicore::XmlWriter - Writes a XML file based on tokens</li>
		<li>uicore::XmlDocument - XML Document class</li>
		<li>uicore::XmlNode - Node in XML document</li>
	</ul>

	<h2>JSON</h2>

	<ul>
		<li>uicore::JsonValue - JSON parse and stringify</li>
	</ul>
		
	<h2>I/O devices</h2>
		
	<ul>
		<li>uicore::IODevice - Input/Output stream abstraction</li>
		<li>uicore::File - Class for opening files</li>
		<li>uicore::MemoryDevice - IODevice for memory buffers</li>
	</ul>
		
	<h2>File systems</h2>

	<ul>
		<li>uicore::DirectoryScanner - List files outside of uicore::FileSystem</li>
		<li>uicore::FilePath - Helper class for working with path strings</li>
		<li>uicore::Directory - Perform basic directory based file operations</li>
	</ul>

	<h2>Zip files</h2>

	<ul>
		<li>uicore::ZLibCompression - Deflate algorithm compressor / decompressor</li>
	</ul>
		
	<h2>Crypto</h2>

	<ul>
		<li>uicore::SHA1, uicore::SHA224, uicore::SHA256, uicore::SHA384, uicore::SHA512, uicore::SHA512_224, uicore::SHA512_256 - SHA Hashing functions</li>
		<li>uicore::MD5 - MD5 Hashing function</li>
		<li>uicore::RSA - RSA encryption/decryption</li>
		<li>uicore::Random - Uses the operating system cryptographically secure pseudorandom number generator</li>
		<li>uicore::AES128_Encrypt, uicore::AES128_Decrypt - AES-128 encryption/decryption running in Cipher Block Chaining mode</li>
		<li>uicore::AES192_Encrypt, uicore::AES192_Decrypt - AES-192 encryption/decryption running in Cipher Block Chaining mode</li>
		<li>uicore::AES256_Encrypt, uicore::AES256_Decrypt - AES-256 encryption/decryption running in Cipher Block Chaining mode</li>
		<li>uicore::Secret - Key class</li>
		<li>uicore::TLSClient - TLS protocol handling for a TLS client</li>
	</ul>

	<h2>Error reporting</h2>

	<ul>
		<li>uicore::CrashReporter - Generates crash reports when the application crashes, or when requested</li>
		<li>uicore::DetectHang - Calls the crash reporter if the constructing thread hangs for more than 30 seconds</li>
		<li>uicore::ExceptionDialog - Displays a modal dialog with exception details</li>
	</ul>

*/
