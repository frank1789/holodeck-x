//! Provides the generic trait Parser

use crate::error::Result;

mod apdl;
mod fbd;
mod frd;

pub use apdl::APDLParser;
pub use fbd::FBDParser;
pub use frd::FRDParser;

pub trait Parser {
    type Output;

    fn parse(&self, input: &str) -> Result<Self::Output>;
}
