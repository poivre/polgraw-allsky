/*
 *                       Yeppp! library implementation
 *                   This file is auto-generated by Peach-Py,
 *        Portable Efficient Assembly Code-generator in Higher-level Python,
 *                  part of the Yeppp! library infrastructure
 * This file is part of Yeppp! library and licensed under the New BSD license.
 * See LICENSE.txt for the full text of the license.
 */

#include <yepBuiltin.h>
#include <yepCore.h>


extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V8s_S8s_Default(const Yep8s *YEP_RESTRICT vPointer, Yep8s *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(vPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(length == 0) {
		return YepStatusInvalidArgument;
	}
	Yep8s minimum = *vPointer++;
	while (--length != 0) {
		const Yep8s v = *vPointer++;
		minimum = yepBuiltin_Min_8s8s_8s(v, minimum);
	}
	*minimumPointer++ = minimum;
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V8u_S8u_Default(const Yep8u *YEP_RESTRICT vPointer, Yep8u *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(vPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(length == 0) {
		return YepStatusInvalidArgument;
	}
	Yep8u minimum = *vPointer++;
	while (--length != 0) {
		const Yep8u v = *vPointer++;
		minimum = yepBuiltin_Min_8u8u_8u(v, minimum);
	}
	*minimumPointer++ = minimum;
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V16s_S16s_Default(const Yep16s *YEP_RESTRICT vPointer, Yep16s *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(vPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(vPointer, sizeof(Yep16s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep16s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(length == 0) {
		return YepStatusInvalidArgument;
	}
	Yep16s minimum = *vPointer++;
	while (--length != 0) {
		const Yep16s v = *vPointer++;
		minimum = yepBuiltin_Min_16s16s_16s(v, minimum);
	}
	*minimumPointer++ = minimum;
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V16u_S16u_Default(const Yep16u *YEP_RESTRICT vPointer, Yep16u *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(vPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(vPointer, sizeof(Yep16u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep16u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(length == 0) {
		return YepStatusInvalidArgument;
	}
	Yep16u minimum = *vPointer++;
	while (--length != 0) {
		const Yep16u v = *vPointer++;
		minimum = yepBuiltin_Min_16u16u_16u(v, minimum);
	}
	*minimumPointer++ = minimum;
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V32s_S32s_Default(const Yep32s *YEP_RESTRICT vPointer, Yep32s *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(vPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(vPointer, sizeof(Yep32s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep32s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(length == 0) {
		return YepStatusInvalidArgument;
	}
	Yep32s minimum = *vPointer++;
	while (--length != 0) {
		const Yep32s v = *vPointer++;
		minimum = yepBuiltin_Min_32s32s_32s(v, minimum);
	}
	*minimumPointer++ = minimum;
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V32u_S32u_Default(const Yep32u *YEP_RESTRICT vPointer, Yep32u *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(vPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(vPointer, sizeof(Yep32u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep32u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(length == 0) {
		return YepStatusInvalidArgument;
	}
	Yep32u minimum = *vPointer++;
	while (--length != 0) {
		const Yep32u v = *vPointer++;
		minimum = yepBuiltin_Min_32u32u_32u(v, minimum);
	}
	*minimumPointer++ = minimum;
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V64s_S64s_Default(const Yep64s *YEP_RESTRICT vPointer, Yep64s *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(vPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(vPointer, sizeof(Yep64s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep64s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(length == 0) {
		return YepStatusInvalidArgument;
	}
	Yep64s minimum = *vPointer++;
	while (--length != 0) {
		const Yep64s v = *vPointer++;
		minimum = yepBuiltin_Min_64s64s_64s(v, minimum);
	}
	*minimumPointer++ = minimum;
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V64u_S64u_Default(const Yep64u *YEP_RESTRICT vPointer, Yep64u *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(vPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(vPointer, sizeof(Yep64u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep64u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(length == 0) {
		return YepStatusInvalidArgument;
	}
	Yep64u minimum = *vPointer++;
	while (--length != 0) {
		const Yep64u v = *vPointer++;
		minimum = yepBuiltin_Min_64u64u_64u(v, minimum);
	}
	*minimumPointer++ = minimum;
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V32f_S32f_Default(const Yep32f *YEP_RESTRICT vPointer, Yep32f *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(vPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(vPointer, sizeof(Yep32f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep32f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(length == 0) {
		return YepStatusInvalidArgument;
	}
	Yep32f minimum = *vPointer++;
	while (--length != 0) {
		const Yep32f v = *vPointer++;
		minimum = yepBuiltin_Min_32f32f_32f(v, minimum);
	}
	*minimumPointer++ = minimum;
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V64f_S64f_Default(const Yep64f *YEP_RESTRICT vPointer, Yep64f *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(vPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(vPointer, sizeof(Yep64f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep64f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(length == 0) {
		return YepStatusInvalidArgument;
	}
	Yep64f minimum = *vPointer++;
	while (--length != 0) {
		const Yep64f v = *vPointer++;
		minimum = yepBuiltin_Min_64f64f_64f(v, minimum);
	}
	*minimumPointer++ = minimum;
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V8sV8s_V8s_Default(const Yep8s *YEP_RESTRICT xPointer, const Yep8s *YEP_RESTRICT yPointer, Yep8s *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	while (length-- != 0) {
		const Yep8s x = *xPointer++;
		const Yep8s y = *yPointer++;
		const Yep8s minimum = yepBuiltin_Min_8s8s_8s(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V8uV8u_V8u_Default(const Yep8u *YEP_RESTRICT xPointer, const Yep8u *YEP_RESTRICT yPointer, Yep8u *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	while (length-- != 0) {
		const Yep8u x = *xPointer++;
		const Yep8u y = *yPointer++;
		const Yep8u minimum = yepBuiltin_Min_8u8u_8u(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V16sV16s_V16s_Default(const Yep16s *YEP_RESTRICT xPointer, const Yep16s *YEP_RESTRICT yPointer, Yep16s *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep16s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep16s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep16s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep16s x = *xPointer++;
		const Yep16s y = *yPointer++;
		const Yep16s minimum = yepBuiltin_Min_16s16s_16s(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V16uV16u_V16u_Default(const Yep16u *YEP_RESTRICT xPointer, const Yep16u *YEP_RESTRICT yPointer, Yep16u *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep16u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep16u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep16u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep16u x = *xPointer++;
		const Yep16u y = *yPointer++;
		const Yep16u minimum = yepBuiltin_Min_16u16u_16u(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V32sV32s_V32s_Default(const Yep32s *YEP_RESTRICT xPointer, const Yep32s *YEP_RESTRICT yPointer, Yep32s *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep32s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep32s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep32s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep32s x = *xPointer++;
		const Yep32s y = *yPointer++;
		const Yep32s minimum = yepBuiltin_Min_32s32s_32s(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V32uV32u_V32u_Default(const Yep32u *YEP_RESTRICT xPointer, const Yep32u *YEP_RESTRICT yPointer, Yep32u *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep32u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep32u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep32u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep32u x = *xPointer++;
		const Yep32u y = *yPointer++;
		const Yep32u minimum = yepBuiltin_Min_32u32u_32u(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V64sV32s_V64s_Default(const Yep64s *YEP_RESTRICT xPointer, const Yep32s *YEP_RESTRICT yPointer, Yep64s *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep64s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep32s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep64s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep64s x = *xPointer++;
		const Yep64s y = *yPointer++;
		const Yep64s minimum = yepBuiltin_Min_64s64s_64s(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V64uV32u_V64u_Default(const Yep64u *YEP_RESTRICT xPointer, const Yep32u *YEP_RESTRICT yPointer, Yep64u *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep64u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep32u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep64u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep64u x = *xPointer++;
		const Yep64u y = *yPointer++;
		const Yep64u minimum = yepBuiltin_Min_64u64u_64u(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V32fV32f_V32f_Default(const Yep32f *YEP_RESTRICT xPointer, const Yep32f *YEP_RESTRICT yPointer, Yep32f *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep32f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep32f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep32f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep32f x = *xPointer++;
		const Yep32f y = *yPointer++;
		const Yep32f minimum = yepBuiltin_Min_32f32f_32f(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V64fV64f_V64f_Default(const Yep64f *YEP_RESTRICT xPointer, const Yep64f *YEP_RESTRICT yPointer, Yep64f *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep64f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep64f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep64f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep64f x = *xPointer++;
		const Yep64f y = *yPointer++;
		const Yep64f minimum = yepBuiltin_Min_64f64f_64f(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V8sS8s_V8s_Default(const Yep8s *YEP_RESTRICT xPointer, Yep8s y, Yep8s *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	while (length-- != 0) {
		const Yep8s x = *xPointer++;
		const Yep8s minimum = yepBuiltin_Min_8s8s_8s(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V8uS8u_V8u_Default(const Yep8u *YEP_RESTRICT xPointer, Yep8u y, Yep8u *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	while (length-- != 0) {
		const Yep8u x = *xPointer++;
		const Yep8u minimum = yepBuiltin_Min_8u8u_8u(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V16sS16s_V16s_Default(const Yep16s *YEP_RESTRICT xPointer, Yep16s y, Yep16s *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep16s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep16s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep16s x = *xPointer++;
		const Yep16s minimum = yepBuiltin_Min_16s16s_16s(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V16uS16u_V16u_Default(const Yep16u *YEP_RESTRICT xPointer, Yep16u y, Yep16u *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep16u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep16u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep16u x = *xPointer++;
		const Yep16u minimum = yepBuiltin_Min_16u16u_16u(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V32sS32s_V32s_Default(const Yep32s *YEP_RESTRICT xPointer, Yep32s y, Yep32s *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep32s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep32s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep32s x = *xPointer++;
		const Yep32s minimum = yepBuiltin_Min_32s32s_32s(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V32uS32u_V32u_Default(const Yep32u *YEP_RESTRICT xPointer, Yep32u y, Yep32u *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep32u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep32u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep32u x = *xPointer++;
		const Yep32u minimum = yepBuiltin_Min_32u32u_32u(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V64sS32s_V64s_Default(const Yep64s *YEP_RESTRICT xPointer, Yep32s y, Yep64s *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep64s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep64s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep64s x = *xPointer++;
		const Yep64s minimum = yepBuiltin_Min_64s64s_64s(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V64uS32u_V64u_Default(const Yep64u *YEP_RESTRICT xPointer, Yep32u y, Yep64u *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep64u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep64u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep64u x = *xPointer++;
		const Yep64u minimum = yepBuiltin_Min_64u64u_64u(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V32fS32f_V32f_Default(const Yep32f *YEP_RESTRICT xPointer, Yep32f y, Yep32f *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep32f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep32f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep32f x = *xPointer++;
		const Yep32f minimum = yepBuiltin_Min_32f32f_32f(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_V64fS64f_V64f_Default(const Yep64f *YEP_RESTRICT xPointer, Yep64f y, Yep64f *YEP_RESTRICT minimumPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep64f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(minimumPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(minimumPointer, sizeof(Yep64f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		const Yep64f x = *xPointer++;
		const Yep64f minimum = yepBuiltin_Min_64f64f_64f(x, y);
		*minimumPointer++ = minimum;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV8sV8s_IV8s_Default(Yep8s *YEP_RESTRICT xPointer, const Yep8s *YEP_RESTRICT yPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	while (length-- != 0) {
		Yep8s x = *xPointer;
		const Yep8s y = *yPointer++;
		x = yepBuiltin_Min_8s8s_8s(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV8uV8u_IV8u_Default(Yep8u *YEP_RESTRICT xPointer, const Yep8u *YEP_RESTRICT yPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	while (length-- != 0) {
		Yep8u x = *xPointer;
		const Yep8u y = *yPointer++;
		x = yepBuiltin_Min_8u8u_8u(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV16sV16s_IV16s_Default(Yep16s *YEP_RESTRICT xPointer, const Yep16s *YEP_RESTRICT yPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep16s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep16s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep16s x = *xPointer;
		const Yep16s y = *yPointer++;
		x = yepBuiltin_Min_16s16s_16s(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV16uV16u_IV16u_Default(Yep16u *YEP_RESTRICT xPointer, const Yep16u *YEP_RESTRICT yPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep16u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep16u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep16u x = *xPointer;
		const Yep16u y = *yPointer++;
		x = yepBuiltin_Min_16u16u_16u(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV32sV32s_IV32s_Default(Yep32s *YEP_RESTRICT xPointer, const Yep32s *YEP_RESTRICT yPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep32s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep32s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep32s x = *xPointer;
		const Yep32s y = *yPointer++;
		x = yepBuiltin_Min_32s32s_32s(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV32uV32u_IV32u_Default(Yep32u *YEP_RESTRICT xPointer, const Yep32u *YEP_RESTRICT yPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep32u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep32u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep32u x = *xPointer;
		const Yep32u y = *yPointer++;
		x = yepBuiltin_Min_32u32u_32u(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV64sV32s_IV64s_Default(Yep64s *YEP_RESTRICT xPointer, const Yep32s *YEP_RESTRICT yPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep64s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep32s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep64s x = *xPointer;
		const Yep64s y = *yPointer++;
		x = yepBuiltin_Min_64s64s_64s(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV64uV32u_IV64u_Default(Yep64u *YEP_RESTRICT xPointer, const Yep32u *YEP_RESTRICT yPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep64u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep32u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep64u x = *xPointer;
		const Yep64u y = *yPointer++;
		x = yepBuiltin_Min_64u64u_64u(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV32fV32f_IV32f_Default(Yep32f *YEP_RESTRICT xPointer, const Yep32f *YEP_RESTRICT yPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep32f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep32f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep32f x = *xPointer;
		const Yep32f y = *yPointer++;
		x = yepBuiltin_Min_32f32f_32f(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV64fV64f_IV64f_Default(Yep64f *YEP_RESTRICT xPointer, const Yep64f *YEP_RESTRICT yPointer, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep64f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	if YEP_UNLIKELY(yPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(yPointer, sizeof(Yep64f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep64f x = *xPointer;
		const Yep64f y = *yPointer++;
		x = yepBuiltin_Min_64f64f_64f(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV8sS8s_IV8s_Default(Yep8s *YEP_RESTRICT xPointer, Yep8s y, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	while (length-- != 0) {
		Yep8s x = *xPointer;
		x = yepBuiltin_Min_8s8s_8s(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV8uS8u_IV8u_Default(Yep8u *YEP_RESTRICT xPointer, Yep8u y, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	while (length-- != 0) {
		Yep8u x = *xPointer;
		x = yepBuiltin_Min_8u8u_8u(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV16sS16s_IV16s_Default(Yep16s *YEP_RESTRICT xPointer, Yep16s y, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep16s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep16s x = *xPointer;
		x = yepBuiltin_Min_16s16s_16s(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV16uS16u_IV16u_Default(Yep16u *YEP_RESTRICT xPointer, Yep16u y, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep16u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep16u x = *xPointer;
		x = yepBuiltin_Min_16u16u_16u(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV32sS32s_IV32s_Default(Yep32s *YEP_RESTRICT xPointer, Yep32s y, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep32s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep32s x = *xPointer;
		x = yepBuiltin_Min_32s32s_32s(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV32uS32u_IV32u_Default(Yep32u *YEP_RESTRICT xPointer, Yep32u y, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep32u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep32u x = *xPointer;
		x = yepBuiltin_Min_32u32u_32u(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV64sS32s_IV64s_Default(Yep64s *YEP_RESTRICT xPointer, Yep32s y, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep64s)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep64s x = *xPointer;
		x = yepBuiltin_Min_64s64s_64s(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV64uS32u_IV64u_Default(Yep64u *YEP_RESTRICT xPointer, Yep32u y, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep64u)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep64u x = *xPointer;
		x = yepBuiltin_Min_64u64u_64u(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV32fS32f_IV32f_Default(Yep32f *YEP_RESTRICT xPointer, Yep32f y, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep32f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep32f x = *xPointer;
		x = yepBuiltin_Min_32f32f_32f(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}

extern "C" YEP_LOCAL_SYMBOL YepStatus _yepCore_Min_IV64fS64f_IV64f_Default(Yep64f *YEP_RESTRICT xPointer, Yep64f y, YepSize length) {
	if YEP_UNLIKELY(xPointer == YEP_NULL_POINTER) {
		return YepStatusNullPointer;
	}
	if YEP_UNLIKELY(yepBuiltin_GetPointerMisalignment(xPointer, sizeof(Yep64f)) != 0) {
		return YepStatusMisalignedPointer;
	}
	while (length-- != 0) {
		Yep64f x = *xPointer;
		x = yepBuiltin_Min_64f64f_64f(x, y);
		*xPointer++ = x;
	}
	return YepStatusOk;
}
