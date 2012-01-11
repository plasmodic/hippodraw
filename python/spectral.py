
"""Spectral analysis stuff.

Exported functions:

- multispec: Compute power and cross spectra.

- wmspec: Compute power and cross spectra, with a window or multitapers."""

num = None
try:
    import numarray as num
    import numarray.fft as FFT
except ImportError:
    import numpy as num
    import numpy.fft as FFT


def multispec(data, doxs=1):

    """Compute power and cross spectra.

    power, cross = multispec(data[, doxs=1])

    data is expected to be a numeric sequence, representing a binned light
    curve.

    If doxs is false or data is one-dimensional, the cross spectra will not
    be calculated as described below, and only the power spectra will be
    returned.

    If data is one-dimensional, its periodogram will be calculated.

    If data is two-dimensional, it will be interpreted as an array of
    one-dimensional curves.  The periodogram will be calculated for each,
    and the cross spectra of all but the first relative to the first.

    If data has more than two dimensions, it will be interpreted as an array
    of two-dimensional sets, and each will be treated as described above."""

    # Figure out what we've got.
    data = num.asarray(data)
    npoints = data.shape[-1]
    if len(data.shape) < 2:
        doxs = 0

    # crunch
    transform = None
    if num.__name__ == 'numarray':
        transform = FFT.real_fft(data)
    else:
        transform = FFT.fft(data)
    conj = num.conjugate(transform)

    # Get the periodogram.
    power = transform * conj
    power = num.array(power.real)

    if doxs:
        # Calculate the cross spectrum.
        cross = transform[..., :1, :] * conj[..., 1:, :] 
        return power, cross
    else:
        return power


def wmspec(data, windows=None, doxs=1):

    """Compute power and cross spectra, with a window or multitapers.

    data and doxs are as for multispec
    (except the default for doxs is different).

    If windows is one-dimensional, it is treated as a windowing function.

    If windows is two-dimensional, it is treated as a set of multitapers."""

    data = num.asarray(data)

    if windows is None:
        windows = [1]
    else:
        windows = num.array(windows, copy=0)
        win_length = windows.shape[-1]
        windows.shape = (-1, win_length)
    nwin = len(windows)

    # subtract the average
    avg = num.sum(data, -1) / data.shape[-1]
    avg = num.asarray(avg)
    data = data - avg[..., num.NewAxis]

    total_power = 0
    if doxs:
        total_cross = 0
    for window in windows:
        spectra = multispec(data * window, doxs=doxs)
        if doxs:
            total_power += spectra[0]
            total_cross += spectra[1]
        else:
            total_power += spectra
    total_power /= nwin
    if doxs:
        total_cross /= nwin
    
    if doxs:
        return total_power, total_cross
    else:
        return total_power


def phaselag(spectrum, coherence, numinbin):

    """Compute phase lags from a cross spectrum.

    lag, error = phaselag(spectrum, coherence, numinbin)

    Inputs:

    spectrum: The cross spectrum.  A complex array.

    coherence: The uncorrected coherence.

    numinbin: The number of raw estimates that went into each frequency bin.

    Outputs:

    lag: The phase of the input spectrum.  In radians.

    error: The standard deviation of the lag.  Also in radians.
    Calculated from eq. (9.52) in Bendat & Piersol 2000."""

    lag = num.arctan2(spectrum.imag, spectrum.real)

    # eq. (9.52) in Bendat & Piersol 2000
    error = num.sqrt((1 - coherence) / (2 * numinbin * coherence))

    return lag, error


def coherence(cross_spectrum, power_spectra, noise=None, numinbin=1,
              subbed=1):

    """Compute coherence from cross and power spectra.

    uncoh, uncoh_err, corcoh = coherence(cross_spectrum, power_spectra,
                                         noise=None, numinbin=1, subbed=1)

    Inputs:

    cross_spectrum, power_spectra: Complex and real arrays, respectively.
    power_spectra has one row for each signal band.
    cross_spectrum has one fewer, and it is assumed that the cross spectra
    of all bands but the first have been taken with respect to the first.

    Will correct[1] for noise if it is supplied.

    numinbin: The number of independent estimates that have gone into
    each frequency bin.

    If subbed is true, noise has already been subtracted from
    the power spectra, and will not be during the calculations.

    Outputs:

    uncoh: The raw coherence.  Eq. (6.51) in Bendat & Piersol 2000.

    uncoh_err:  Standard deviation in the raw coherence.
    Eq. (9.81) in Bendat & Piersol 2000.

    corcoh: The corrected coherence.  Eq. (8) in Vaughan & Nowak 1997.

    [1] Vaughan & Nowak 1997, ApJ 474:L43"""

    cross_spectrum = num.asarray(cross_spectrum)
    power_spectra = num.asarray(power_spectra)
    if noise is not None:
        correct = 1
        noise = num.asarray(noise)

    if subbed:
        unsubtracted = power_spectra + noise
        if correct:
            subtracted = power_spectra
    else:
        unsubtracted = power_spectra
        if correct:
            subtracted = power_spectra - noise

    s_one = unsubtracted[..., :1, :]
    s_two = unsubtracted[..., 1:, :]
    numerator = num.absolute(cross_spectrum) ** 2
    uncorrected = numerator / (s_one * s_two)
    # Sometimes floating-point issues cause these limits to be violated.
    uncorrected = num.minimum(uncorrected, 1.0)
    uncorrected = num.maximum(uncorrected, 0.0)

    # error in uncorrected coherence
    # eq. (9.81) in Bendat & Piersol 2000
    varcoh = 2 * uncorrected * (1 - uncorrected) ** 2 / numinbin 
    uncoh_error = num.sqrt(varcoh)

    if correct:
        s_one = subtracted[..., :1, :]
        s_two = subtracted[..., 1:, :]
        n_one = noise[..., :1, :]
        n_two = noise[..., 1:, :]
        n_square = (s_one * n_two + n_one * s_two + n_one * n_two) / numinbin
        numerator -= n_square
        corrected = numerator / (s_one * s_two)

    if correct:
        return uncorrected, uncoh_error, corrected
    else:
        return uncorrected, uncoh_error

