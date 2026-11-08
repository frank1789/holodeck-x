use std::env;
use std::path::PathBuf;

/// Returns the home directory
///
/// # Example
///
/// ```
/// use directories::get_home_dir;
/// let home_dir = get_home_dir();
/// ```
pub fn get_home_dir() -> Option<PathBuf> {
    #[cfg(target_os = "windows")]
    {
        env::var("USERPROFILE").ok().map(PathBuf::from)
    }

    #[cfg(any(target_os = "linux", target_os = "macos"))]
    {
        env::var("HOME").ok().map(PathBuf::from)
    }

    #[cfg(not(any(target_os = "windows", target_os = "linux", target_os = "macos")))]
    {
        env::var("HOME").ok().map(PathBuf::from)
    }
}
