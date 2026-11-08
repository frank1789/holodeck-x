//! Provides the FBD parser

use super::Parser;
use crate::error::{ParseError, Result};

#[derive(Debug, Default, Clone)]
pub struct FBDParser {}

impl Parser for FBDParser {
    type Output = String;

    fn parse(&self, input: &str) -> Result<Self::Output> {
        Ok("Success".to_string())
    }
}
