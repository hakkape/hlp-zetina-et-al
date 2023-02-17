# https://stackoverflow.com/questions/1471994/what-is-setup-py
from setuptools import setup

setup(
    name="hlp_zetina",
    version="1.0",
    description="Python wrapper of the HLP solver by Zetina et al.",
    author="Carlos Zetina (solver), Alexander Helber (wrapper)",
    author_email="helber@or.rwth-aachen.de",
    packages=["hlp_zetina"]  # same as name
    # install_requires=["wheel", "bar", "greek"],  # external packages as dependencies
)
